//
// Created by ivanedo on 26-06-15.
//

#include "Solution.hpp"
#include <iostream>
#include <algorithm>
#include <memory>
#include <iterator>

Solution::Solution(Instance *instance): instance(instance), load_cost(0), balance_cost(0), process_move_cost(0), service_move_cost(0), machine_move_cost(0) {
    //  set empty assignment
    Assignments assignments(instance->processes.size(), -1);
    set_assignments(assignments);
}

Solution::~Solution() {}

int64_t Solution::get_load_cost() {
    return load_cost;
}

int64_t Solution::get_balance_cost() {
    return balance_cost;
}

int64_t Solution::get_process_move_cost() {
    return process_move_cost;
}

int64_t Solution::get_service_move_cost() {
    return service_move_cost;
}

int64_t Solution::get_machine_move_cost() {
    return machine_move_cost;
}

int64_t Solution::get_solution_cost() {
    return load_cost + balance_cost + process_move_cost + service_move_cost + machine_move_cost;
}

Assignments Solution::get_assignments() {
    return assignments;
}

void Solution::set_assignments(Assignments assignments) {
    this->assignments = assignments;
}

void Solution::print_assignments() {
    std::cout << "Assignments:" << std::endl;
    std::copy(assignments.begin(), assignments.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
}

void Solution::read_solution_from_file(std::ifstream &in) {
    Assignments initial_assignments;
    unsigned int assignment;
    while(in >> assignment) {
        initial_assignments.push_back(assignment);
    }
    set_assignments(initial_assignments);
}

void Solution::write_solution_to_file(std::ofstream &out) {
    std::copy(assignments.begin(), assignments.end(), std::ostream_iterator<unsigned int>(out, " "));
    out << std::endl;
}

void Solution::print() {
    std::cout << "load_cost: " << get_load_cost() << ", ";
    std::cout << "balance_cost: " << get_balance_cost() <<  ", ";
    std::cout << "process_move_cost: " << get_process_move_cost() <<  ", ";
    std::cout << "service_move_cost: " << get_service_move_cost() <<  ", ";
    std::cout << "machine_move_cost: " << get_machine_move_cost() <<  ", ";
    std::cout << "total_cost: " << get_solution_cost() << std::endl;
}


//COSTS CALCULATION

int64_t Solution::calc_load_cost(unsigned int machine_id, unsigned int resource_id) {
    Machine *machine = instance->get_machine(machine_id);
    return (machine->get_usage(resource_id) > machine->get_scapacity(resource_id)) ?
           (machine->get_usage(resource_id) - machine->get_scapacity(resource_id))
           : 0;
}

int64_t Solution::calc_balance_cost(unsigned int machine_id, unsigned int balance_id) {
    Machine *machine = instance->get_machine(machine_id);
    Balance *balance = instance->get_balance(balance_id);

    int64_t capacity_r1 = machine->get_capacity(balance->get_resource_id(1));
    int64_t usage_r1 = machine->get_usage(balance->get_resource_id(1));
    int64_t available_r1 = capacity_r1 - usage_r1;

    int64_t capacity_r2 = machine->get_capacity(balance->get_resource_id(2));
    int64_t usage_r2 = machine->get_usage(balance->get_resource_id(2));
    int64_t available_r2 = capacity_r2 - usage_r2;

    unsigned int target = balance->get_target();

    int64_t bcost = target * available_r1 - available_r2;

    return (bcost > 0) ? bcost : 0;
}

int64_t Solution::calc_process_move_cost(unsigned int process_id) {
    Process *process = instance->get_process(process_id);
    return (process->get_initial_machine_id() != process->get_current_machine_id()) ? process->get_process_move_cost() : 0;
}

unsigned int Solution::calc_service_move_cost(unsigned int service_id) {
    return instance->get_service(service_id)->get_num_moved_processes();
}

int64_t Solution::calc_machine_move_cost(unsigned int process_id) {
    Process *process = instance->get_process(process_id);
    unsigned int initial_machine_id = process->get_initial_machine_id();
    unsigned int current_machine_id = process->get_current_machine_id();

    return (initial_machine_id == current_machine_id) ? 0 : instance->get_machine(initial_machine_id)->get_machine_move_cost(current_machine_id);
}

int64_t Solution::calc_machine_load_cost(unsigned int machine_id) {
    int64_t cost = 0;
    std::deque<Resource*>::iterator resource_iter;  //on every resource
    for (resource_iter = instance->resources.begin(); resource_iter != instance->resources.end(); ++resource_iter) {
        cost += calc_load_cost(machine_id, (*resource_iter)->get_id()) * (*resource_iter)->get_weight_load_cost();
    }
    return cost;
}

int64_t Solution::calc_machine_balance_cost(unsigned int machine_id) {
    int64_t cost = 0;
    std::deque<Balance*>::iterator balance_iter;  //on every balance
    for (balance_iter = instance->balances.begin(); balance_iter != instance->balances.end(); ++balance_iter) {
        cost += (calc_balance_cost(machine_id, (*balance_iter)->get_id()) * (*balance_iter)->get_weight_balance_cost());
    }
    return cost;
}

int64_t Solution::calc_total_load_cost() {
    int64_t cost = 0;
    std::deque<Machine*>::iterator machine_iter;
    for (machine_iter = instance->machines.begin(); machine_iter != instance->machines.end(); ++machine_iter) {
        cost += calc_machine_load_cost((*machine_iter)->get_id());
    }
    return cost;
}

int64_t Solution::calc_total_balance_cost() {
    int64_t cost = 0;
    std::deque<Machine*>::iterator machine_iter;
    for (machine_iter = instance->machines.begin(); machine_iter != instance->machines.end(); ++machine_iter) {
        cost += calc_machine_balance_cost((*machine_iter)->get_id());
    }
    return cost;
}

int64_t Solution::calc_total_process_move_cost() {
    int64_t cost = 0;
    std::deque<Process*>::iterator process_iter;
    for (process_iter = instance->processes.begin(); process_iter != instance->processes.end(); ++process_iter) {
        cost += Solution::calc_process_move_cost((*process_iter)->get_id());
    }
    return cost * instance->get_weight_process_move_cost();
}

int64_t Solution::calc_total_service_move_cost() {
    unsigned int max = 0;
    std::deque<Service*>::iterator service_iter;
    for (service_iter = instance->services.begin(); service_iter != instance->services.end(); ++service_iter) {
        unsigned int cost = calc_service_move_cost((*service_iter)->get_id());
        if(cost > max) {
            max = cost;
        }
    }
    max_num_of_moved_processes = max;

    return max_num_of_moved_processes * instance->get_weight_service_move_cost();
}

int64_t Solution::calc_total_machine_move_cost() {
    int64_t cost = 0;
    std::deque<Process*>::iterator process_iter;
    for (process_iter = instance->processes.begin(); process_iter != instance->processes.end(); ++process_iter) {
        cost += calc_machine_move_cost((*process_iter)->get_id());
    }
    return cost * instance->get_weight_machine_move_cost();
}


// UPDATE COSTS

void Solution::update_solution_load_cost() {
    load_cost = calc_total_load_cost();
}

void Solution::update_solution_balance_cost() {
    balance_cost = calc_total_balance_cost();
}

void Solution::update_solution_process_move_cost() {
    process_move_cost = calc_total_process_move_cost();
}

void Solution::update_solution_service_move_cost() {
    service_move_cost = calc_total_service_move_cost();
}

void Solution::update_solution_machine_move_cost() {
    machine_move_cost = calc_total_machine_move_cost();
}

void Solution::update_solution_costs() {
    update_solution_load_cost();
    update_solution_balance_cost();
    update_solution_process_move_cost();
    update_solution_service_move_cost();
    update_solution_machine_move_cost();
}


// CHECK CONSTRAINST WITH SHIFT MOVEMENT

bool Solution::check_capacity_with_shift(unsigned int process_id, unsigned int machine_id) {
    Process *process = instance->get_process(process_id);
    Machine *new_machine = instance->get_machine(machine_id);

    for (unsigned int i = 0; i < instance->resources.size(); ++i) {
        int32_t new_usage = new_machine->get_usage(i) + process->get_requirement(i);
        if(new_usage > new_machine->get_capacity(i)) {
            return false;
        }
    }

    return true;
}

bool Solution::check_conflict_with_shift(unsigned int process_id, unsigned int machine_id) {
    //que cada proceso se ejecute en una máquina distinta
    //la maquina a la cual se reasignará el proceso, es distinta de la inicial
    Process *process = instance->get_process(process_id);
    Service *service = instance->get_service(process->get_service_id());
    //search new machine in service's used machines list, if not found, there is no problem
    return !service->has_machine(machine_id);
}

bool Solution::check_spread_with_shift(unsigned int process_id, unsigned int machine_id) {
    Process *process = instance->get_process(process_id);
    Service *service = instance->get_service(process->get_service_id());
    unsigned int current_process_location = process->get_location_id();
    unsigned int new_machine_location = instance->get_machine(machine_id)->get_location_id();


    if(service->has_location(new_machine_location) && new_machine_location != current_process_location) {
        //primero determinar la cantidad de procesos del servicio ejecutándose en la localización actual del proceso.
        unsigned int num_of_processes_in_current_location = 0;
        for (unsigned int i = 0; i < service->processes.size(); ++i) {
            unsigned int process_location = instance->get_process(service->processes[i])->get_location_id();
            if(process_location == current_process_location) {
                num_of_processes_in_current_location++;
            }
        }

        if(num_of_processes_in_current_location < 2) {
            //al reasignar, la localización actual queda huerfana, por lo que el # localizaciones utilizadas por el
            //servicio disminuye en 1.
            if(service->locations.size() - 1 < service->get_spread_min()) {
                return false;
            }
        }
    }
    return true;
}

bool Solution::check_dependency_with_shift(unsigned int process_id, unsigned int machine_id) {
    //S_a servicio al cual pertenece el proceso
    Process *process = instance->get_process(process_id);
    Service *service = instance->get_service(process->get_service_id());
    unsigned int new_machine_neighborhood = instance->get_machine(machine_id)->get_neighborhood_id();
    unsigned int current_process_neighborhood = process->get_neighborhood_id();

    //En cualquier caso, ya sea S_a depende de S_b o S_c depende de S_a, si el proceso se mantiene en el mismo vecindario
    //la solución se mantiene factible
    if(new_machine_neighborhood == current_process_neighborhood) {
        return true;
    }

    //S_a depende de S_b
    //Si el vecindario nuevo, ya se encontraba usado por el servicio, la solución se mantiene factible,
    //en caso contrario, debemos chequear de que las dependencias utilicen ese vecindario nuevo
    if(!service->dependencies.empty() && !service->has_neighborhood(new_machine_neighborhood)) {
        for (unsigned int i = 0; i < service->dependencies.size(); ++i) {
            Service *dependency = instance->get_service(service->dependencies[i]);
            if(!dependency->has_neighborhood(new_machine_neighborhood)){
                return false;
            }
        }
    }

    //S_c depende de S_a
    if(!service->dependents.empty()) {

        unsigned int num_of_processes_in_current_neighborhood = 0;
        for (unsigned int i = 0; i < service->processes.size(); ++i) {
            unsigned int process_neighborhood = instance->get_process(service->processes[i])->get_neighborhood_id();
            if(process_neighborhood == current_process_neighborhood) {
                num_of_processes_in_current_neighborhood++;
            }
        }

        if(num_of_processes_in_current_neighborhood < 2) {
            return false;
        }
    }

    return true;
}

bool Solution::check_transient_usage_with_shift(unsigned int process_id, unsigned int machine_id) {
    //considerar si la nueva máquina es la inicial

    Process *process = instance->get_process(process_id);
    Machine *new_machine = instance->get_machine(machine_id);

    bool is_the_new_machine_the_initial_one = (new_machine->get_id() == process->get_initial_machine_id());

    for (unsigned int i = 0; i < instance->resources.size(); ++i) {

        if(instance->get_resource(i)->is_transient()) {

            int64_t transient_usage = (is_the_new_machine_the_initial_one) ?
                                                new_machine->get_transient_usage(i) - process->get_requirement(i)
                                                : new_machine->get_transient_usage(i);

            int64_t new_usage = new_machine->get_usage(i) + process->get_requirement(i) + transient_usage;

            if(new_usage > new_machine->get_capacity(i)) {
                return false;
            }
        }
    }

    return true;
}

bool Solution::check_shift(unsigned int process_id, unsigned int machine_id) {
    return (check_capacity_with_shift(process_id, machine_id)
            && check_conflict_with_shift(process_id, machine_id)
            && check_spread_with_shift(process_id, machine_id)
            && check_dependency_with_shift(process_id, machine_id)
            && check_transient_usage_with_shift(process_id, machine_id));
}


// CALCULATION OF COSTS VARIATION

int64_t Solution::get_load_cost_with_process(unsigned int process_id, unsigned int machine_id) {
    Machine* machine = instance->get_machine(machine_id);

    if (machine->has_process(process_id)) {
        return calc_machine_load_cost(machine_id);
    }

    Process *process = instance->get_process(process_id);
    int64_t cost = 0;
    std::deque<Resource*>::iterator resource_iter;  //on every resource
    for (resource_iter = instance->resources.begin(); resource_iter != instance->resources.end(); ++resource_iter) {

        int64_t usage_with_process = machine->get_usage((*resource_iter)->get_id()) + process->get_requirement((*resource_iter)->get_id());
        int64_t resource_cost = 0;

        if(usage_with_process > machine->get_scapacity((*resource_iter)->get_id())) {
            resource_cost = usage_with_process - machine->get_scapacity((*resource_iter)->get_id());
        }

        cost += (resource_cost * (*resource_iter)->get_weight_load_cost());
    }
    return cost;
}

int64_t Solution::get_load_cost_without_process(unsigned int process_id, unsigned int machine_id) {
    Machine* machine = instance->get_machine(machine_id);

    if(!machine->has_process(process_id)) {
        return calc_machine_load_cost(machine_id);
    }

    Process *process = instance->get_process(process_id);
    int64_t cost = 0;
    std::deque<Resource*>::iterator resource_iter;  //on every resource
    for (resource_iter = instance->resources.begin(); resource_iter != instance->resources.end(); ++resource_iter) {

        int64_t usage_without_process = machine->get_usage((*resource_iter)->get_id()) - process->get_requirement((*resource_iter)->get_id());
        int64_t resource_cost = 0;

        if(usage_without_process > machine->get_scapacity((*resource_iter)->get_id())) {
            resource_cost = usage_without_process - machine->get_scapacity((*resource_iter)->get_id());
        }

        cost += (resource_cost * (*resource_iter)->get_weight_load_cost());
    }
    return cost;
}

int64_t Solution::get_balance_cost_with_process(unsigned int process_id, unsigned int machine_id) {
    Machine *machine = instance->get_machine(machine_id);

    if(machine->has_process(process_id)) {
        return calc_machine_balance_cost(machine_id);
    }

    Process *process = instance->get_process(process_id);
    int64_t cost = 0;
    std::deque<Balance*>::iterator balance_iter;  //on every balance
    for (balance_iter = instance->balances.begin(); balance_iter != instance->balances.end(); ++balance_iter) {

        int64_t capacity_r1 = machine->get_capacity((*balance_iter)->get_resource_id(1));
        int64_t usage_r1 = machine->get_usage((*balance_iter)->get_resource_id(1)) + process->get_requirement((*balance_iter)->get_resource_id(1));
        int64_t available_r1 = capacity_r1 - usage_r1;

        int64_t capacity_r2 = machine->get_capacity((*balance_iter)->get_resource_id(2));
        int64_t usage_r2 = machine->get_usage((*balance_iter)->get_resource_id(2)) + process->get_requirement((*balance_iter)->get_resource_id(2));

        int64_t available_r2 = capacity_r2 - usage_r2;

        int64_t bcost = ((*balance_iter)->get_target() * available_r1 - available_r2) * (*balance_iter)->get_weight_balance_cost();

        cost += (bcost > 0) ? bcost : 0;
    }
    return cost;

}

int64_t Solution::get_balance_cost_without_process(unsigned int process_id, unsigned int machine_id) {
    Machine *machine = instance->get_machine(machine_id);

    if(!machine->has_process(process_id)) {
        return calc_machine_balance_cost(machine_id);
    }

    Process *process = instance->get_process(process_id);
    int64_t cost = 0;
    std::deque<Balance*>::iterator balance_iter;  //on every balance
    for (balance_iter = instance->balances.begin(); balance_iter != instance->balances.end(); ++balance_iter) {

        int64_t capacity_r1 = machine->get_capacity((*balance_iter)->get_resource_id(1));
        int64_t usage_r1 = machine->get_usage((*balance_iter)->get_resource_id(1)) - process->get_requirement((*balance_iter)->get_resource_id(1));

        int64_t available_r1 = capacity_r1 - usage_r1;

        int64_t capacity_r2 = machine->get_capacity((*balance_iter)->get_resource_id(2));
        int64_t usage_r2 = machine->get_usage((*balance_iter)->get_resource_id(2)) - process->get_requirement((*balance_iter)->get_resource_id(2));

        int64_t available_r2 = capacity_r2 - usage_r2;

        int64_t bcost = ((*balance_iter)->get_target() * available_r1 - available_r2) * (*balance_iter)->get_weight_balance_cost();

        cost += (bcost > 0) ? bcost : 0;
    }

    return cost;

}

int64_t Solution::calc_delta_load_cost_with_shift(unsigned int process_id, unsigned int machine_id) {
    //se necesita calcular la variación en el costo de carga al realizar el shift del proceso
    //primero, calcular el estado actual, es decir, el costo de carga en la máquina antigua (old machine) con el proceso
    //en ella
    Process *process = instance->get_process(process_id);
    Machine *old_machine = instance->get_machine(process->get_current_machine_id());
    Machine *new_machine = instance->get_machine(machine_id);

    int64_t delta_load_cost_old_machine = get_load_cost_with_process(process->get_id(), old_machine->get_id()) - get_load_cost_without_process(process->get_id(), old_machine->get_id());

    int64_t delta_load_cost_new_machine = get_load_cost_with_process(process->get_id(), new_machine->get_id()) - get_load_cost_without_process(process->get_id(), new_machine->get_id());

    int64_t delta = delta_load_cost_new_machine - delta_load_cost_old_machine;

    return delta;
    //si el delta es negativo, el descenso en el costo de la máquina antigua es mayor que el aumento que se da máquina nueva
}

int64_t Solution::calc_delta_balance_cost_with_shift(unsigned int process_id, unsigned int machine_id) {

    Process *process = instance->get_process(process_id);
    Machine *old_machine = instance->get_machine(process->get_current_machine_id());
    Machine *new_machine = instance->get_machine(machine_id);

    int64_t delta_bcost_old_machine = get_balance_cost_without_process(process->get_id(), old_machine->get_id()) - get_balance_cost_with_process(process->get_id(), old_machine->get_id());

    int64_t delta_bcost_new_machine = get_balance_cost_with_process(process->get_id(), new_machine->get_id()) - get_balance_cost_without_process(process->get_id(), new_machine->get_id());

    return delta_bcost_new_machine + delta_bcost_old_machine;
}

int64_t Solution::calc_delta_process_move_cost_with_shift(unsigned int process_id, unsigned int machine_id) {
    Process *process = instance->get_process(process_id);
    Machine *old_machine = instance->get_machine(process->get_current_machine_id());
    Machine *new_machine = instance->get_machine(machine_id);

    //tres casos:

    //  se devuelve el proceso a su máquina inicial
    if(new_machine->get_id() == process->get_initial_machine_id()) {
        return -(process->get_process_move_cost() * instance->get_weight_process_move_cost());

    //  se reasigna el proceso desde su máquina inicial a otra distinta
    } else if(old_machine->get_id() == process->get_initial_machine_id()) {
        return (process->get_process_move_cost() * instance->get_weight_process_move_cost());

    //  se reasigna el proceso desde una máquina distinta a la inicial a otra cualquiera, es decir el proceso ya se movió
    //  desde su máquina inicial, por lo que el costo ya se ve reflejado en el process_move_cost
    } else {
        return 0;
    }
}

int64_t Solution::calc_delta_service_move_cost_with_shift(unsigned int process_id, unsigned int machine_id) {
    Process *process = instance->get_process(process_id);
    Machine *old_machine = instance->get_machine(process->get_current_machine_id());
    Machine *new_machine = instance->get_machine(machine_id);
    Service *service = instance->get_service(process->get_service_id());

    //tres casos:

    //  se devuelve el proceso a su máquina inicial
    if(new_machine->get_id() == process->get_initial_machine_id()) {
        unsigned int service_moved_process = service->get_num_moved_processes() - 1;
        if(max_num_of_moved_processes - 1 > service_moved_process) {
            return 0;

        } else if (max_num_of_moved_processes - 1 == service_moved_process) {
            //tenemos que buscar si otro servicio tiene el mismo número de proceso movidos, haciendo que el máximo se mantenga
            std::deque<Service*>::iterator service_iter;
            for (service_iter = instance->services.begin(); service_iter != instance->services.end(); ++service_iter) {
                if((*service_iter)->get_num_moved_processes() == max_num_of_moved_processes && (*service_iter)->get_id() != service->get_id()) {
                    return 0; //hay otros servicio que establecen el máximo
                }
            }
            return -(instance->get_weight_service_move_cost()); //si no hay otros servicios que establecen el máximo
        }

        //  se reasigna el proceso desde su máquina inicial a otra distinta
    } else if(old_machine->get_id() == process->get_initial_machine_id()) {
        unsigned int service_moved_process = service->get_num_moved_processes() + 1;
        //luego de mover el proceso, la variable service_moved_process se incrementa en uno
        return (service_moved_process == max_num_of_moved_processes + 1) ? instance->get_weight_service_move_cost() : 0;

        //  se reasigna el proceso desde una máquina distinta a la inicial a otra cualquiera, es decir el proceso ya se movió
        //  desde su máquina inicial, por lo que el costo ya se ve reflejado en el service_move_cost
    }
    //la cantidad de procesos movidos del servicio dueño del proceso, se mantiene
    return 0;


}

int64_t Solution::calc_delta_machine_move_cost_with_shift(unsigned int process_id, unsigned int machine_id) {
    Process *process = instance->get_process(process_id);
    Machine *old_machine = instance->get_machine(process->get_current_machine_id());
    Machine *new_machine = instance->get_machine(machine_id);

    //tres casos:

    //  se devuelve el proceso a su máquina inicial, new machine es la original
    if(new_machine->get_id() == process->get_initial_machine_id()) {
        return -(new_machine->get_machine_move_cost(old_machine->get_id()) * instance->get_weight_machine_move_cost());

        //  se reasigna el proceso desde su máquina inicial a otra distinta, old machine es la original
    } else if(old_machine->get_id() == process->get_initial_machine_id()) {
        return (old_machine->get_machine_move_cost(new_machine->get_id()) * instance->get_weight_machine_move_cost());

        //  se reasigna el proceso desde una máquina distinta a la inicial a otra cualquiera, es decir el proceso ya se movió
        //  desde su máquina inicial, por lo que el costo ya se ve reflejado en el process_move_cost
    } else {// old machine and new machine son distintas de la original
        Machine *original_machine = instance->get_machine(process->get_initial_machine_id());
        int32_t delta = -(original_machine->get_machine_move_cost(old_machine->get_id()))//removimos el antiguo costo
                         + (original_machine->get_machine_move_cost(new_machine->get_id())); //y agregamos el nuevo

        return delta;
    }
}

int64_t Solution::calc_delta_cost_with_shift(unsigned int process_id, unsigned int machine_id) {
    return (
            calc_delta_load_cost_with_shift(process_id, machine_id)
            + calc_delta_balance_cost_with_shift(process_id, machine_id)
            + calc_delta_process_move_cost_with_shift(process_id, machine_id)
            + calc_delta_service_move_cost_with_shift(process_id, machine_id)
            + calc_delta_machine_move_cost_with_shift(process_id, machine_id)
    );
}



// SHIFT MOVEMENT

void Solution::shift_process(unsigned int process_id, unsigned int machine_id) {
    Process *process = instance->get_process(process_id);
    Machine *old_machine = instance->get_machine(process->get_current_machine_id());
    Machine *new_machine = instance->get_machine(machine_id);

    bool is_the_old_machine_the_initial_one = (process->get_current_machine_id() == process->get_initial_machine_id());

    process->set_current_machine_id(new_machine->get_id());

    //for the old machine, update the usages
   for (unsigned int i = 0; i < instance->resources.size(); ++i) {
        int32_t new_usage = old_machine->get_usage(i) - process->get_requirement(i);
        old_machine->set_usage(i, new_usage);
        if(is_the_old_machine_the_initial_one && instance->get_resource(i)->is_transient()) {
            int32_t new_transient_usage = old_machine->get_transient_usage(i) + process->get_requirement(i);
            old_machine->set_transient_usage(i, new_transient_usage);
        }
    }
    old_machine->remove_process(process_id);

    bool is_the_new_machine_the_initial_one = (new_machine->get_id() == process->get_initial_machine_id());

    new_machine->add_process(process_id);

    //for the new machine, update usages
    for (unsigned int j = 0; j < instance->resources.size(); ++j) {
        int32_t new_usage = new_machine->get_usage(j) + process->get_requirement(j);
        new_machine->set_usage(j, new_usage);
        if(is_the_new_machine_the_initial_one && instance->get_resource(j)->is_transient()) {
            int32_t new_transient_usage = new_machine->get_transient_usage(j) - process->get_requirement(j);
            new_machine->set_transient_usage(j, new_transient_usage);
        }
    }

    //para el servicio dueño del proceso
    //  quitar la maquina inicial y agregar la nueva a la lista de máquinas
    //  actualizar la lista de vecindarios
    //  actualizar la lista de localizaciones
    Service *service = instance->get_service(process->get_service_id());
    service->remove_machine(old_machine->get_id());
    service->add_machine(new_machine->get_id());

    if(is_the_old_machine_the_initial_one) {
        service->increment_moved_processes();

    } else if(is_the_new_machine_the_initial_one){
        service->decrement_moved_processes();
    }

    unsigned int old_process_neighborhood = process->get_neighborhood_id();
    unsigned int new_machine_neighborhood = new_machine->get_neighborhood_id();

    if(old_process_neighborhood != new_machine_neighborhood) {
        process->set_neighborhood_id(new_machine_neighborhood);

        if(!service->has_neighborhood(new_machine_neighborhood)) {
            service->add_neighborhood(new_machine_neighborhood);
        }

        unsigned int num_of_processes_in_old_neighborhood = 0;
        for (unsigned int i = 0; i < service->processes.size(); ++i) {
            if(instance->get_process(service->processes[i])->get_neighborhood_id() == old_process_neighborhood) {
                num_of_processes_in_old_neighborhood++;
            }
        }

        if(num_of_processes_in_old_neighborhood == 0) {
            service->remove_neighborhood(old_process_neighborhood);
        }
    }

    unsigned int old_process_location = process->get_location_id();
    unsigned int new_machine_location = new_machine->get_location_id();

    if(old_process_location != new_machine_location) {
        process->set_location_id(new_machine_location);

        if(!service->has_location(new_machine_location)) {
            service->add_location(new_machine_location);
        }

        unsigned int num_of_processes_in_old_location = 0;
        for (unsigned int i = 0; i < service->processes.size(); ++i) {
            if(instance->get_process(service->processes[i])->get_location_id() == old_process_location) {
                num_of_processes_in_old_location++;
            }
        }

        if(num_of_processes_in_old_location == 0) {
            service->remove_location(old_process_location);
        }
    }

    this->assignments[process_id] = machine_id;

    //para la solucion, recalcular los costos
    update_solution_costs();
}


// FUNCTIONS FOR GREEDY PROCEDURE

void Solution::assign_process(unsigned int process_id, unsigned int machine_id) {
    Process *process = instance->get_process(process_id);
    Machine *machine = instance->get_machine(machine_id);

    machine->add_process(process_id);

    //usage setting for machine

    for (unsigned int i = 0; i < instance->resources.size(); ++i) {
        int32_t new_usage = machine->get_usage(i) + process->get_requirement(i);
        machine->set_usage(i, new_usage);
    }

    process->set_initial_machine_id(machine_id);
    process->set_current_machine_id(machine_id);
    process->set_location_id(machine->get_location_id());
    process->set_neighborhood_id(machine->get_neighborhood_id());

    Service *service = instance->get_service(process->get_service_id());


    service->add_machine(machine_id);

    if(!service->has_neighborhood(machine->get_neighborhood_id())) {
        service->add_neighborhood(machine->get_neighborhood_id());
    }

    if(!service->has_location(machine->get_location_id())) {
        service->add_location(machine->get_location_id());
    }

    this->assignments[process_id] = machine_id;


}

bool Solution::relaxed_check_assignment(unsigned int process_id, unsigned int machine_id) {
    return (
            check_capacity_with_assignment(process_id, machine_id)
            && check_conflict_with_assignment(process_id, machine_id)
    );
}

bool Solution::check_assignment(unsigned int process_id, unsigned int machine_id) {
    return (
            check_capacity_with_assignment(process_id, machine_id)
            && check_conflict_with_assignment(process_id, machine_id)
            && check_spread_with_assignment(process_id, machine_id)
            && check_dependency_with_assignment(process_id, machine_id)
    );
}

bool Solution::check_capacity_with_assignment(unsigned int process_id, unsigned int machine_id) {
    return check_capacity_with_shift(process_id, machine_id);
}

bool Solution::check_conflict_with_assignment(unsigned int process_id, unsigned int machine_id) {
    return check_conflict_with_shift(process_id, machine_id);
}

bool Solution::check_spread_with_assignment(unsigned int process_id, unsigned int machine_id) {
    Process *process = instance->get_process(process_id);
    Service *service = instance->get_service(process->get_service_id());
    unsigned int machine_location = instance->get_machine(machine_id)->get_location_id();

    return service->locations.size() <= service->get_spread_min() || !service->has_location(machine_location);

}

bool Solution::check_dependency_with_assignment(unsigned int process_id, unsigned int machine_id) {
    Process *process = instance->get_process(process_id);
    Service *service = instance->get_service(process->get_service_id());
    unsigned int machine_neighborhood = instance->get_machine(machine_id)->get_neighborhood_id();

    if(!service->has_neighborhood(machine_neighborhood)) {
        for (unsigned int i = 0; i < service->dependencies.size(); ++i) {
            Service *dependency = instance->get_service(service->dependencies[i]);
            if(!dependency->has_neighborhood(machine_neighborhood)){
                return false;
            }
        }
    }

    return true;
}
