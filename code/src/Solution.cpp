//
// Created by ivanedo on 26-06-15.
//

#include "Solution.h"
#include <iostream>
#include <algorithm>
#include <memory>
#include <iterator>

Solution::Solution() {

}

Solution::Solution(Instance *instance) {
    this->instance = instance;
}

Solution::~Solution() {
    std::cout << "Deleting solution..." << std::endl;
}

unsigned long int Solution::get_load_cost() {
    return load_cost;
}

unsigned long int Solution::get_balance_cost() {
    return balance_cost;
}

unsigned long int Solution::get_process_move_cost() {
    return process_move_cost;
}

unsigned long int Solution::get_service_move_cost() {
    return service_move_cost;
}

unsigned long int Solution::get_machine_move_cost() {
    return machine_move_cost;
}

unsigned long int Solution::get_solution_cost() {
    return load_cost + balance_cost + process_move_cost + service_move_cost + machine_move_cost;
}

void Solution::read_solution_from_file(std::ifstream &in) {
    unsigned int assignment;
    while(in >> assignment) {
        assignments.push_back(assignment);
    }
}

void Solution::write_solution_to_file(std::ofstream &out) {
    std::copy(assignments.begin(), assignments.end(), std::ostream_iterator<unsigned int>(out, " "));
    out << std::endl;
}

void Solution::print() {
    std::cout << "Assignments:" << std::endl;
    std::copy(assignments.begin(), assignments.end(), std::ostream_iterator<unsigned int>(std::cout, " "));
    std::cout << "Costs:" << std::endl;
    std::cout << "Load cost: " << get_load_cost() << std::endl;
    std::cout << "Balance cost: " << get_balance_cost() << std::endl;
    std::cout << "Process move cost: " << get_process_move_cost() << std::endl;
    std::cout << "Service move cost: " << get_service_move_cost() << std::endl;
    std::cout << "Machine move cost: " << get_machine_move_cost() << std::endl;
}

Assignments Solution::get_assignments() {
    return assignments;
}

//COSTS CALCULATION

unsigned long int Solution::calc_load_cost(unsigned int machine_id, unsigned int resource_id) {
    Machine *machine = instance->get_machine(machine_id);
    return (machine->get_usage(resource_id) > machine->get_scapacity(resource_id)) ?
           (machine->get_usage(resource_id) - machine->get_scapacity(resource_id))
           : 0;
}

unsigned long int Solution::calc_balance_cost(unsigned int machine_id, unsigned int balance_id) {
    Machine *machine = instance->get_machine(machine_id);
    Balance *balance = instance->get_balance(balance_id);

    unsigned long int capacity_r1 = machine->get_capacity(balance->get_resource_id(1));
    unsigned long int usage_r1 = machine->get_usage(balance->get_resource_id(1));
    unsigned long int available_r1 = capacity_r1 - usage_r1;

    unsigned long int capacity_r2 = machine->get_capacity(balance->get_resource_id(2));
    unsigned long int usage_r2 = machine->get_usage(balance->get_resource_id(2));
    unsigned long int available_r2 = capacity_r2 - usage_r2;

    unsigned int target = balance->get_target();

    return ((target * available_r1 - available_r2) > 0) ? (target * available_r1 - available_r2) : 0;
}

unsigned long int Solution::calc_process_move_cost(unsigned int process_id) {
    Process *process = instance->get_process(process_id);
    return (process->get_initial_machine_id() != process->get_current_machine_id()) ? process->get_process_move_cost() : 0;
}

unsigned long int Solution::calc_service_move_cost(unsigned int service_id) {
    return instance->get_service(service_id)->get_num_moved_processes();
}

unsigned long int Solution::calc_machine_move_cost(unsigned int process_id) {
    Process *process = instance->get_process(process_id);
    unsigned int initial_machine_id = process->get_initial_machine_id();
    unsigned int current_machine_id = process->get_current_machine_id();

    return (initial_machine_id == current_machine_id) ? 0 : instance->get_machine(initial_machine_id)->get_machine_move_cost(current_machine_id);
}

unsigned long int Solution::calc_machine_load_cost(unsigned int machine_id) {
    unsigned long int cost = 0;
    std::deque<Resource*>::iterator resource_iter;  //on every resource
    for (resource_iter = instance->resources.begin(); resource_iter != instance->resources.end(); ++resource_iter) {
        cost += calc_load_cost(machine_id, (*resource_iter)->get_id()) * (*resource_iter)->get_weight_load_cost();
    }
    return cost;
}

unsigned long int Solution::calc_machine_balance_cost(unsigned machine_id) {
    unsigned long int cost = 0;
    std::deque<Balance*>::iterator balance_iter;  //on every balance
    for (balance_iter = instance->balances.begin(); balance_iter != instance->balances.end(); ++balance_iter) {
        cost += Solution::calc_balance_cost(machine_id, (*balance_iter)->get_id()) * (*balance_iter)->get_weight_balance_cost();
    }
    return cost;
}

unsigned long int Solution::calc_total_load_cost() {
    unsigned long int cost = 0;
    std::deque<Machine*>::iterator machine_iter;
    for (machine_iter = instance->machines.begin(); machine_iter != instance->machines.end(); ++machine_iter) {
        cost += calc_machine_load_cost((*machine_iter)->get_id());
    }
    return cost;
}

unsigned long int Solution::calc_total_balance_cost() {
    unsigned long int cost = 0;
    std::deque<Machine*>::iterator machine_iter;
    for (machine_iter = instance->machines.begin(); machine_iter != instance->machines.end(); ++machine_iter) {
        cost += Solution::calc_machine_balance_cost((*machine_iter)->get_id());
    }
    return cost;
}

unsigned long int Solution::calc_total_process_move_cost() {
    unsigned long int cost = 0;
    std::deque<Process*>::iterator process_iter;
    for (process_iter = instance->processes.begin(); process_iter != instance->processes.end(); ++process_iter) {
        cost += Solution::calc_process_move_cost((*process_iter)->get_id());
    }
    return cost * instance->get_weight_process_move_cost();
}

unsigned long int Solution::calc_total_service_move_cost() {
    unsigned long int max = 0;
    std::deque<Service*>::iterator service_iter;
    for (service_iter = instance->services.begin(); service_iter != instance->services.end(); ++service_iter) {
        unsigned long int cost = Solution::calc_service_move_cost((*service_iter)->get_id());
        if(cost > max) {
            max = cost;
        }
    }
    return max;
}

unsigned long int Solution::calc_total_machine_move_cost() {
    unsigned long int cost = 0;
    std::deque<Process*>::iterator process_iter;
    for (process_iter = instance->processes.begin(); process_iter != instance->processes.end(); ++process_iter) {
        cost += Solution::calc_machine_move_cost((*process_iter)->get_id());
    }
    return cost * instance->get_weight_machine_move_cost();
}

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
    max_num_of_moved_processes = calc_total_service_move_cost();
    service_move_cost = max_num_of_moved_processes * instance->get_weight_service_move_cost();
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

// Constraints

//capacity constraint
bool Solution::check_capacity_constraint(unsigned int machine_id, unsigned int resource_id) {
    Machine *machine = instance->get_machine(machine_id);
    return (machine->get_capacity(resource_id) >= machine->get_usage(resource_id));
}

//conflict constraint
bool Solution::check_conflict_constraint(unsigned int service_id) {
    Service *service = instance->get_service(service_id);
    return (service->machines.size() == service->processes.size());
}

//spread constraint
bool Solution::check_spread_constraint(unsigned int service_id) {
    Service *service = instance->get_service(service_id);
    return (service->locations.size() >= service->get_spread_min());
}

//dependency constraint
bool Solution::check_dependency_constraint(unsigned int service_id) {
    Service *service = instance->get_service(service_id);
    for (int i = 0; i < service->neighborhoods.size(); ++i) {
        for (int j = 0; j < service->dependencies.size(); ++j) {
            Service *dependency = instance->get_service(service->dependencies[j]);
            if(!dependency->has_neighborhood(service->neighborhoods[i])) {
                //if not found
                return false;
            }
        }
    }
    return true;
}

bool Solution::check_all_capacity_constraints() {
    std::deque<Machine*>::iterator machine_iter;
    std::deque<Resource*>::iterator resource_iter;

    for (machine_iter = instance->machines.begin(); machine_iter != instance->machines.end(); ++machine_iter) {
        for (resource_iter = instance->resources.begin(); resource_iter != instance->resources.end(); ++resource_iter) {
            if(!check_capacity_constraint((*machine_iter)->get_id(), (*resource_iter)->get_id())) {
                return false;
            }
        }
    }
    return true;
}

bool Solution::check_all_conflict_constraints() {
    std::deque<Service*>::iterator service_iter;
    for (service_iter = instance->services.begin(); service_iter != instance->services.end() ; ++service_iter) {
        if(!check_conflict_constraint((*service_iter)->get_id())) {
            return false;
        }
    }
    return true;
}

bool Solution::check_all_spread_constraints() {
    std::deque<Service*>::iterator service_iter;
    for (service_iter = instance->services.begin(); service_iter != instance->services.end() ; ++service_iter) {
        if(!check_spread_constraint((*service_iter)->get_id())) {
            return false;
        }
    }
    return true;
}

bool Solution::check_all_dependency_constraints() {
    std::deque<Service*>::iterator service_iter;
    for (service_iter = instance->services.begin(); service_iter != instance->services.end() ; ++service_iter) {
        if(!check_dependency_constraint((*service_iter)->get_id())) {
            return false;
        }
    }
    return true;
}

bool Solution::check_solution() {
    return (check_all_capacity_constraints()
            && check_all_conflict_constraints()
            && check_all_spread_constraints()
            && check_all_dependency_constraints());
}

bool Solution::check_capacity_with_shift(unsigned int process_id, unsigned int machine_id) {
    Process *process = instance->get_process(process_id);
    Machine *new_machine = instance->get_machine(machine_id);

    for (unsigned int i = 0; i < instance->resources.size(); ++i) {
        unsigned long int new_usage = new_machine->get_usage(i) + process->get_requirement(i);
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

//num of used locations larger than service's spread min
bool Solution::check_spread_with_shift(unsigned int process_id, unsigned int machine_id) {
    Process *process = instance->get_process(process_id);
    Service *service = instance->get_service(process->get_service_id());
    unsigned int machine_location = instance->get_machine(machine_id)->get_location_id();
    //si la nueva localizacion ya se encuentra en la lista del servicio, la solución se mantiene factible,
    //excepto cuando el proceso es el único ejecutándose en esa localización,
    if(service->has_location(machine_location)) {//Si encuentra la localización dentro de las utilizadas por el servicio

        //determinar si el proceso es el único en esa localización
        //buscar dentro de la lista de procesos del servicio
        unsigned int num_processes_in_location = 0;
        for (int i = 0; i < service->processes.size(); ++i) {
            unsigned int process_location = instance->get_process(service->processes[i])->get_location_id();
            if(process_location == machine_location) {
                num_processes_in_location++;
            }
        }

        if(num_processes_in_location < 2 && (service->locations.size() - 1 < service->get_spread_min())) {//solo un proceso en esa localización
            return false;
        }
    }
    //si no está en la lista, hay dos situaciones considerando la localización antigua:
    //  el proceso era el único presente en esa localización antigua
    //      el servicio deja de utilizar la localización antigua, pero agrega la nueva, manteniéndose la cantidad de localizaciones usadas,
    //      ergo se mantiene la factibilidad
    //  el proceso no era el único presente en esa localización antigua
    //      en este caso se incrementa la cantidad de localizaciones utilizadas por el servicio, la solución se mantiene factible
    // según este análisis, una solución que ya es factible, se mantiene en ese estado con este movimiento
    return true;
}

bool Solution::check_dependency_with_shift(unsigned int process_id, unsigned int machine_id) {
    //analizar los dos casos, las dependencias y los dependientes
    //S_a servicio al cual pertenece el proceso
    Process *process = instance->get_process(process_id);
    Service *service = instance->get_service(process->get_service_id());
    unsigned int machine_neighborhood = instance->get_machine(machine_id)->get_neighborhood_id();

    if(!service->has_neighborhood(machine_neighborhood)) {//no encontrado, nuevo neighborhood a agregar a la lista del servicio
        //S_a depende de S_b
        for (int i = 0; i < service->dependencies.size(); ++i) {
            Service *dependency = instance->get_service(service->dependencies[i]);
            if(!dependency->has_neighborhood(machine_neighborhood)){
                return false;
            }
        }
        //S_c depende de S_a
        //la solución se mantiene factible, los S_c de forma previa no necesitan que exista un proceso de
        //S_a en el nuevo vecindario

    } else {//encontrado
        //S_a depende de S_b
        //Sea cual sea el neighborhood, S_b ya tiene un proceso en el, puesto que comienza con una solución factible

        //S_c depende de S_a
        //Verificar que el vecindario original del proceso, no quede huerfano, es decir sin un proceso de S_a ejecutandose en el
        unsigned int num_process_in_neighborhood = 0;
        for (int i = 0; i < service->processes.size(); ++i) {
            unsigned int process_neighborhood = instance->processes[service->processes[i]]->get_neighborhood_id();
            if(process_neighborhood == machine_neighborhood) {
                num_process_in_neighborhood++;
            }
        }

        if(num_process_in_neighborhood < 2 && !service->dependents.empty()) {
        //Hay servicios que dependen de S_a, buscar si tienen algún proceso en ese vecindario
            for (int i = 0; i < service->dependents.size(); ++i) {
                Service *dependent = instance->services[service->dependents[i]];
                if(std::find(dependent->neighborhoods.begin(), dependent->neighborhoods.end(), machine_neighborhood) != dependent->neighborhoods.end()) {
                    //if found
                    return false;
                }
            }
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

            unsigned long int transient_usage = (is_the_new_machine_the_initial_one) ?
                                                new_machine->get_transient_usage(i) - process->get_requirement(i)
                                                : new_machine->get_transient_usage(i);

            unsigned long int new_usage = new_machine->get_usage(i) + process->get_requirement(i) + transient_usage;

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

Usages Solution::get_usage_with_process(unsigned int process_id, unsigned int machine_id) {
    Machine *machine = instance->get_machine(machine_id);
    if(machine->has_process(process_id)) {
        return machine->usages;
    }
    Process *process = instance->get_process(process_id);
    Usages usages = machine->usages;
    for (unsigned int i = 0; i < usages.size(); ++i) {
        usages[i] += process->get_requirement(i);
    }
    return usages;
}

Usages Solution::get_usage_without_process(unsigned int process_id, unsigned int machine_id) {
    Machine *machine = instance->get_machine(machine_id);
    if(!machine->has_process(process_id)) {
        return machine->usages;
    }
    Process *process = instance->get_process(process_id);
    Usages usages = machine->usages;
    for (unsigned int i = 0; i < usages.size(); ++i) {
        usages[i] -= process->get_requirement(i);
    }
    return usages;
}

unsigned long int Solution::get_load_cost_with_process(unsigned int process_id, unsigned int machine_id) {
    Machine* machine = instance->get_machine(machine_id);

    if (machine->has_process(process_id)) {
        return calc_machine_load_cost(machine_id);
    }

    Usages usages = get_usage_with_process(process_id, machine_id);

    unsigned long int cost = 0;
    std::deque<Resource*>::iterator resource_iter;  //on every resource
    for (resource_iter = instance->resources.begin(); resource_iter != instance->resources.end(); ++resource_iter) {
        unsigned long int resource_cost = (usages[(*resource_iter)->get_id()] > machine->get_scapacity((*resource_iter)->get_id())
                                           ? usages[(*resource_iter)->get_id()] - machine->get_scapacity((*resource_iter)->get_id())
                                           : 0);
        cost += resource_cost * (*resource_iter)->get_weight_load_cost();
    }
    return cost;
}

unsigned long int Solution::get_load_cost_without_process(unsigned int process_id, unsigned int machine_id) {
    Machine* machine = instance->get_machine(machine_id);

    if(!machine->has_process(process_id)) {
        return calc_machine_load_cost(machine_id);
    }

    Usages usages = get_usage_without_process(process_id, machine_id);

    unsigned long int cost = 0;
    std::deque<Resource*>::iterator resource_iter;  //on every resource
    for (resource_iter = instance->resources.begin(); resource_iter != instance->resources.end(); ++resource_iter) {
        unsigned long int resource_cost = (usages[(*resource_iter)->get_id()] > machine->get_scapacity((*resource_iter)->get_id())
                                           ? usages[(*resource_iter)->get_id()] - machine->get_scapacity((*resource_iter)->get_id())
                                           : 0);
        cost += resource_cost * (*resource_iter)->get_weight_load_cost();
    }
    return cost;
}

unsigned long int Solution::get_balance_cost_with_process(unsigned int process_id, unsigned int machine_id) {
    Machine *machine = instance->get_machine(machine_id);

    if(machine->has_process(process_id)) {
        return calc_machine_balance_cost(machine_id);
    }

    Process *process = instance->get_process(process_id);
    unsigned long int cost = 0;
    std::deque<Balance*>::iterator balance_iter;  //on every balance
    for (balance_iter = instance->balances.begin(); balance_iter != instance->balances.end(); ++balance_iter) {

        unsigned long int capacity_r1 = machine->get_capacity((*balance_iter)->get_resource_id(1));
        unsigned long int usage_r1 = machine->get_usage((*balance_iter)->get_resource_id(1))
                                + process->get_requirement((*balance_iter)->get_resource_id(1));
        unsigned long int available_r1 = capacity_r1 - usage_r1;

        unsigned long int capacity_r2 = machine->get_capacity((*balance_iter)->get_resource_id(2));
        unsigned long int usage_r2 = machine->get_usage((*balance_iter)->get_resource_id(2))
                                + process->get_requirement((*balance_iter)->get_resource_id(2));;

        unsigned long int available_r2 = capacity_r2 - usage_r2;

        unsigned int target = (*balance_iter)->get_target();

        cost += ((target * available_r1 - available_r2) > 0) ? (target * available_r1 - available_r2) : 0;
    }
    return cost;

}

unsigned long int Solution::get_balance_cost_without_process(unsigned int process_id, unsigned int machine_id) {
    Machine *machine = instance->get_machine(machine_id);

    if(!machine->has_process(process_id)) {
        return calc_machine_balance_cost(machine_id);
    }

    Process *process = instance->get_process(process_id);
    unsigned long int cost = 0;
    std::deque<Balance*>::iterator balance_iter;  //on every balance
    for (balance_iter = instance->balances.begin(); balance_iter != instance->balances.end(); ++balance_iter) {

        unsigned long int capacity_r1 = machine->get_capacity((*balance_iter)->get_resource_id(1));
        unsigned long int usage_r1 = machine->get_usage((*balance_iter)->get_resource_id(1))
                                - process->get_requirement((*balance_iter)->get_resource_id(1));

        unsigned long int available_r1 = capacity_r1 - usage_r1;

        unsigned long int capacity_r2 = machine->get_capacity((*balance_iter)->get_resource_id(2));
        unsigned long int usage_r2 = machine->get_usage((*balance_iter)->get_resource_id(2))
                                - process->get_requirement((*balance_iter)->get_resource_id(2));;

        unsigned long int available_r2 = capacity_r2 - usage_r2;

        unsigned int target = (*balance_iter)->get_target();

        cost += ((target * available_r1 - available_r2) > 0) ? (target * available_r1 - available_r2) : 0;
    }

    return cost;

}

long int Solution::calc_delta_load_cost_with_shift(unsigned int process_id, unsigned int machine_id) {
    //se necesita calcular la variación en el costo de carga al realizar el shift del proceso
    //primero, calcular el estado actual, es decir, el costo de carga en la máquina antigua (old machine) con el proceso
    //en ella
    Process *process = instance->get_process(process_id);
    Machine *old_machine = instance->get_machine(process->get_current_machine_id());
    Machine *new_machine = instance->get_machine(machine_id);

    long int delta_load_cost_old_machine = get_load_cost_without_process(process->get_id(), old_machine->get_id()) //final state, lower cost
                                           - get_load_cost_with_process(process->get_id(), old_machine->get_id()); //initial state, bigger cost

    long int delta_load_cost_new_machine = get_load_cost_without_process(process->get_id(), new_machine->get_id()) //final state, bigger cost
                                           - get_load_cost_with_process(process->get_id(), new_machine->get_id()); //initial state, lower cost


    long int delta = delta_load_cost_new_machine - delta_load_cost_old_machine;
    return delta;
    //si el delta es negativo, el descenso en el costo de la máquina antigua es mayor que el aumento que se da máquina nueva
}

long int Solution::calc_delta_balance_cost_with_shift(unsigned int process_id, unsigned int machine_id) {

    Process *process = instance->get_process(process_id);
    Machine *old_machine = instance->get_machine(process->get_current_machine_id());
    Machine *new_machine = instance->get_machine(machine_id);

    long int delta_balance_cost_old_machine = get_balance_cost_without_process(process->get_id(), old_machine->get_id())
                                              - get_balance_cost_with_process(process->get_id(), old_machine->get_id());

    long int delta_balance_cost_new_machine = get_balance_cost_with_process(process->get_id(), new_machine->get_id())
                                              - get_balance_cost_without_process(process->get_id(), new_machine->get_id());

    long int delta = delta_balance_cost_new_machine - delta_balance_cost_old_machine;
    return delta;
    //si el delta es negativo, el descenso en el costo de la máquina antigua es mayor que el aumento que se da máquina nueva
}

long int Solution::calc_delta_process_move_cost_with_shift(unsigned int process_id, unsigned int machine_id) {
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

long int Solution::calc_delta_service_move_cost_with_shift(unsigned int process_id, unsigned int machine_id) {
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
    } else {
        //la cantidad de procesos movidos del servicio dueño del proceso, se mantiene
        return 0;
    }

}

long int Solution::calc_delta_machine_move_cost_with_shift(unsigned int process_id, unsigned int machine_id) {
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
        long int delta = -(original_machine->get_machine_move_cost(old_machine->get_id()))//removimos el antiguo costo
                         + (original_machine->get_machine_move_cost(new_machine->get_id())); //y agregamos el nuevo

        return delta;
    }
}

long int Solution::calc_delta_cost_with_shift(unsigned int process_id, unsigned int machine_id) {
    return (calc_delta_load_cost_with_shift(process_id, machine_id)
            + calc_delta_balance_cost_with_shift(process_id, machine_id)
            + calc_delta_process_move_cost_with_shift(process_id, machine_id)
            + calc_delta_service_move_cost_with_shift(process_id, machine_id)
            + calc_delta_machine_move_cost_with_shift(process_id, machine_id));
}

//AL REALIZAR UNA ASIGNACIÓN COMPROBAR LOS RECURSOS
//este método se ejecutará con previo chequeo del movimiento, es decir, se realizará si solo produce una solución factible.
void Solution::shift_process(unsigned int process_id, unsigned int machine_id) {
    Process *process = instance->get_process(process_id);
    Machine *old_machine = instance->get_machine(process->get_current_machine_id());
    Machine *new_machine = instance->get_machine(machine_id);

    bool is_the_old_machine_the_initial_one = (process->get_current_machine_id() == process->get_initial_machine_id());

    process->set_current_machine_id(new_machine->get_id());

    //for the old machine, update the usages
   for (unsigned int i = 0; i < instance->resources.size(); ++i) {
        unsigned long int new_usage = old_machine->get_usage(i) - process->get_requirement(i);
        old_machine->set_usage(i, new_usage);
        if(is_the_old_machine_the_initial_one && instance->get_resource(i)->is_transient()) {
           old_machine->transient_usages[i] += process->get_requirement(i);
        }
    }
    old_machine->remove_process(process_id);


    bool is_the_new_machine_the_initial_one = (new_machine->get_id() == process->get_initial_machine_id());

    new_machine->add_process(process_id);

    //for the new machine, update usages
    for (unsigned int j = 0; j < instance->resources.size(); ++j) {
        unsigned long int new_usage = new_machine->get_usage(j) + process->get_requirement(j);
        new_machine->set_usage(j, new_usage);
        if(is_the_new_machine_the_initial_one && instance->get_resource(j)->is_transient()) {
            new_machine->transient_usages[j] -= process->get_requirement(j);
        }
    }

    //para el servicio dueño del proceso
    //  quitar la maquina inicial y agregar la nueva a la lista de máquinas
    //  actualizar la lista de vecindarios
    //  actualizar la lista de localizaciones
    Service *service = instance->get_service(process->get_service_id());
    service->remove_machine(old_machine->get_id());
    service->add_machine(new_machine->get_id());


    if(process->get_neighborhood_id() != new_machine->get_neighborhood_id()) {
        unsigned int old_process_neighborhood = process->get_neighborhood_id();
        process->set_neighborhood_id(new_machine->get_neighborhood_id());

        if(!service->has_neighborhood(new_machine->get_neighborhood_id())) {
            service->add_neighborhood(new_machine->get_neighborhood_id());
        }

        MachineList::iterator machine_iter;
        bool old_process_neighborhood_found = false;
        for (machine_iter = service->machines.begin(); machine_iter != service->machines.end(); ++machine_iter) {
            if(instance->get_machine(*machine_iter)->get_neighborhood_id() == old_process_neighborhood) {
                old_process_neighborhood_found = true;
            }
        }

        if(!old_process_neighborhood_found) {//si no fue encontrado, se remueve
            service->remove_neighborhood(old_process_neighborhood);
        }
    }

    if(process->get_location_id() != new_machine->get_location_id()) {
        unsigned int old_process_location = process->get_location_id();
        process->set_location_id(new_machine->get_location_id());

        if(!service->has_location(new_machine->get_location_id())) {
            service->add_location(new_machine->get_location_id());
        }

        MachineList::iterator machine_iter;
        bool old_process_location_found = false;
        for (machine_iter = service->machines.begin(); machine_iter != service->machines.end(); ++machine_iter) {
            if(instance->get_machine(*machine_iter)->get_location_id() == old_process_location) {
                old_process_location_found = true;
            }
        }

        if(!old_process_location_found) {//si no fue encontrado, se remueve
            service->remove_neighborhood(old_process_location);
        }
    }

    this->assignments[process_id] = machine_id;

    //para la solucion, recalcular los costos
    update_solution_costs();
}

unsigned int Solution::get_current_assignment(unsigned int process_id) {
    return assignments[process_id];
}