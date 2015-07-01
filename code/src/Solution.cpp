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
    std::cout << "Load cost: " << load_cost << std::endl;
    std::cout << "Balance cost: " << balance_cost << std::endl;
    std::cout << "Process move cost: " << process_move_cost << std::endl;
    std::cout << "Service move cost: " << service_move_cost << std::endl;
    std::cout << "Machine move cost: " << machine_move_cost << std::endl;
}


Assignments Solution::get_assignments() {
    return assignments;
}


//COSTS CALCULATION

unsigned long int Solution::calc_load_cost(unsigned int machine_id, unsigned int resource_id) {
    unsigned int usage = instance->machines[machine_id]->usages[resource_id];
    unsigned int scapacity = instance->machines[machine_id]->safety_capacities[resource_id];

    return (usage > scapacity) ? (usage - scapacity) : 0;
}

unsigned long int Solution::calc_balance_cost(unsigned int machine_id, unsigned int balance_id) {
    Machine *machine = instance->machines.at(machine_id);
    Balance *balance = instance->balances.at(balance_id);

    unsigned int capacity_r1 = machine->capacities.at(balance->get_resource_id(1));
    unsigned int usage_r1 = machine->usages[balance->get_resource_id(1)];
    unsigned int available_r1 = capacity_r1 - usage_r1;

    unsigned int capacity_r2 = machine->capacities.at(balance->get_resource_id(2));
    unsigned int usage_r2 = machine->usages[balance->get_resource_id(2)];
    unsigned int available_r2 = capacity_r2 - usage_r2;

    unsigned int target = balance->get_target();

    return ((target * available_r1 - available_r2) > 0) ? (target * available_r1 - available_r2) : 0;
}

unsigned long int Solution::calc_process_move_cost(unsigned int process_id) {
    Process *process = instance->processes[process_id];
    return (process->get_initial_machine_id() != process->get_current_machine_id()) ? process->get_process_move_cost() : 0;
}

unsigned long int Solution::calc_service_move_cost(unsigned int service_id) {
    unsigned int num_moved_processes = 0;
    Service *service = instance->services[service_id];
    for(std::deque<unsigned int>::iterator it = service->processes.begin(); it != service->processes.end(); ++it) {
        unsigned int initial_machine = instance->processes[*it]->get_initial_machine_id();
        unsigned int current_machine = instance->processes[*it]->get_current_machine_id();
        if(initial_machine != current_machine) {
            num_moved_processes += 1;
        }
    }
    return num_moved_processes;
}

unsigned long int Solution::calc_machine_move_cost(unsigned int process_id) {
    Process *process = instance->processes[process_id];
    unsigned int initial_machine_id = process->get_initial_machine_id();
    unsigned int current_machine_id = process->get_current_machine_id();

    if(initial_machine_id == current_machine_id) {
        return 0;
    } else {
        Machine *initial_machine = instance->machines[initial_machine_id];
        return initial_machine->mmc[current_machine_id];
    }
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
    return cost * instance->weight_process_move_cost;
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
    return max * instance->weight_service_move_cost;
}


unsigned long int Solution::calc_total_machine_move_cost() {
    unsigned long int cost = 0;
    std::deque<Process*>::iterator process_iter;
    for (process_iter = instance->processes.begin(); process_iter != instance->processes.end(); ++process_iter) {
        cost += Solution::calc_machine_move_cost((*process_iter)->get_id());
    }
    return cost * instance->weight_machine_move_cost;
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

// Constraints

//capacity constraint
bool Solution::check_capacity_constraint(unsigned int machine_id, unsigned int resource_id) {
    unsigned int capacity = instance->machines[machine_id]->capacities[resource_id];
    unsigned int usage = instance->machines[machine_id]->usages[resource_id];

    return (capacity >= usage);
}

//conflict constraint
bool Solution::check_conflict_constraint(unsigned int service_id) {
    unsigned long int num_used_machines = instance->services[service_id]->machines.size();
    unsigned long int num_processes = instance->services[service_id]->processes.size();

    return (num_processes == num_used_machines);
}

//spread constraint
bool Solution::check_spread_constraint(unsigned int service_id) {
    unsigned long int num_used_locations = instance->services[service_id]->locations.size();
    unsigned int spread_min = instance->services[service_id]->get_spread_min();

    return (num_used_locations >= spread_min);
}

//dependency constraint
bool Solution::check_dependency_constraint(unsigned int service_id) {
    NeighborhoodList &neighborhoods = instance->services[service_id]->neighborhoods;
    ServiceList &dependencies = instance->services[service_id]->dependencies;
    for (int i = 0; i < neighborhoods.size(); ++i) {
        for (int j = 0; j < dependencies.size(); ++j) {
            Service *dependency = instance->services[dependencies[j]];
            if(std::find(dependency->neighborhoods.begin(), dependency->neighborhoods.end(), neighborhoods[i]) == dependency->neighborhoods.end()) {
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
    return (check_all_capacity_constraints() && check_all_conflict_constraints() && check_all_spread_constraints() && check_all_dependency_constraints());
}


bool Solution::check_capacity_with_shift(unsigned int process_id, unsigned int machine_id) {
    Process *process = instance->processes[process_id];
    Machine *new_machine = instance->machines[machine_id];

    for (unsigned int i = 0; i < instance->resources.size(); ++i) {
        unsigned int new_usage = new_machine->get_usage(i) + process->get_requirement(i);
        if(new_usage > new_machine->get_capacity(i)) {
            return false;
        }
    }

    return true;
}

bool Solution::check_conflict_with_shift(unsigned int process_id, unsigned int machine_id) {
    //que cada proceso se ejecute en una máquina distinta
    //la maquina a la cual se reasignará el proceso, es distinta de la inicial
    Process *process = instance->processes[process_id];
    Service *service = instance->services[process->get_service_id()];
    //search new machine in service's used machines list, if not found, there is no problem
    MachineList::iterator position = std::find(service->machines.begin(), service->machines.end(), machine_id);
    if (position != service->machines.end()) {
        return false;
    } // == vector.end() means the element was not found

    return true;
}

//num of used locations larger than service's spread min
bool Solution::check_spread_with_shift(unsigned int process_id, unsigned int machine_id) {
    Process *process = instance->processes[process_id];
    Service *service = instance->services[process->get_service_id()];
    unsigned int machine_location = instance->machines[machine_id]->get_location_id();
    //si la nueva localizacion ya se encuentra en la lista del servicio, la solución se mantiene factible,
    //excepto cuando el proceso es el único ejecutándose en esa localización,
    LocationList::iterator position = std::find(service->locations.begin(), service->locations.end(), machine_location);
    if(position != service->locations.end()) {//Si encuentra la localización dentro de las utilizadas por el servicio

        //determinar si el proceso es el único en esa localización
        //buscar dentro de la lista de procesos del servicio
        unsigned int num_processes_in_location = 0;
        for (int i = 0; i < service->processes.size(); ++i) {
            unsigned int process_location = instance->processes[service->processes[i]]->get_location_id();
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
    Process *process = instance->processes[process_id];
    Service *service = instance->services[process->get_service_id()];
    unsigned int machine_neighborhood = instance->machines[machine_id]->get_neighborhood_id();

    NeighborhoodList::iterator position = std::find(service->neighborhoods.begin(), service->neighborhoods.end(), machine_neighborhood);
    bool neighborhood_found = (position != service->neighborhoods.end());

    if(!neighborhood_found) {//no encontrado, nuevo neighborhood a agregar a la lista del servicio
        //S_a depende de S_b
        for (int i = 0; i < service->dependencies.size(); ++i) {
            Service *dependency = instance->services[service->dependencies[i]];
            if(std::find(dependency->neighborhoods.begin(), dependency->neighborhoods.end(), machine_neighborhood) == dependency->neighborhoods.end()) {
                //if not found
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
    Process *process = instance->processes[process_id];
    Machine *new_machine = instance->machines[machine_id];

    for (unsigned int i = 0; i < instance->resources.size(); ++i) {
        if(instance->resources[i]->is_transient()) {
            unsigned int new_usage = new_machine->get_usage(i) + process->get_requirement(i) + new_machine->get_transient_usage(i);
            if(new_usage > new_machine->get_capacity(i)) {
                return false;
            }
        }
    }

    return true;
}

bool Solution::check_shift(unsigned int process_id, unsigned int machine_id) {
    return (check_capacity_with_shift(process_id, machine_id) && check_conflict_with_shift(process_id, machine_id) && check_spread_with_shift(process_id, machine_id) && check_dependency_with_shift(process_id, machine_id) && check_transient_usage_with_shift(process_id, machine_id));
}

//AL REALIZAR UNA ASIGNACIÓN COMPROBAR LOS RECURSOS
//este método se ejecutará con previo chequeo del movimiento, es decir, se realizará si solo produce una solución factible.
void Solution::shift_process(unsigned int process_id, unsigned int machine_id) {
    Process *process = instance->processes[process_id];
    Machine *old_machine = instance->machines[process->get_initial_machine_id()];
    Machine *new_machine = instance->machines[machine_id];
    //al reasignar
    //para el proceso,
    //  cambiar el current_machine_id, neighborhood_id, location_id
    process->set_current_machine_id(machine_id);

    //caching old location and neighborhood
    unsigned int old_location = process->get_location_id();
    unsigned int old_neighborhood = process->get_neighborhood_id();
    process->set_location_id(new_machine->get_location_id());
    process->set_neighborhood_id(new_machine->get_neighborhood_id());


    //para la maquina inicial,
    //  recalcular los usos para cada recurso
    //  quitar al proceso de la lista de procesos,
    //  tomar en consideración los recursos de uso transitorio
    for (unsigned int i = 0; i < instance->resources.size(); ++i) {
        unsigned int new_usage = old_machine->get_usage(i) - process->get_requirement(i);
        old_machine->set_usage(i, new_usage);
        if(instance->resources[i]->is_transient()) {
           old_machine->transient_usages[i] += process->get_requirement(i);
        }
    }
    old_machine->remove_process(process_id);


    //para la maquina nueva
    //  agregar el id del proceso a la lista de procesos
    //  recalcular los usos
    new_machine->add_process(process_id);
    for (unsigned int j = 0; j < instance->resources.size(); ++j) {
        unsigned int new_usage = new_machine->get_usage(j) + process->get_requirement(j);
        new_machine->set_usage(j, new_usage);
    }

    //para el servicio dueño del proceso
    //  quitar la maquina inicial y agregar la nueva a la lista de máquinas
    //  actualizar la lista de vecindarios
    //  actualizar la lista de localizaciones
    Service *service = instance->services[process->get_service_id()];
    service->remove_machine(old_machine->get_id());
    service->add_machine(new_machine->get_id());



    MachineList service_machines = service->machines;
    //del nuevo conjunto de máquinas usadas por el servicio, sacar el neighborhood_id y el location_id
    //si el neighborhood y el location actual no se encuentran, quitarlos y agregar los nuevos
    //comprobar además si el nuevo neighborhood y location del proceso se encuentran ya en las listas del servicio correspondiente
    bool neighborhood_found = false;
    bool location_found = false;
    for (int k = 0; k < service_machines.size(); ++k) {
        unsigned int neighborhood = instance->processes[service_machines[k]]->get_neighborhood_id();
        unsigned int location = instance->processes[service_machines[k]]->get_location_id();

        if(process->get_neighborhood_id() == neighborhood) {
            neighborhood_found = true;
        }

        if(process->get_location_id() == location) {
            location_found = true;
        }
    }

    //if neighborhood not found
    if(!neighborhood_found) {
        //remove old one
        service->remove_neighborhood(old_neighborhood);
        //add new one
        service->add_neighborhood(process->get_neighborhood_id());
    }

    if(!location_found) {
        //remove old one
        service->remove_location(old_location);
        //add new one
        service->add_location(process->get_location_id());
    }

    this->assignments[process_id] = machine_id;

    //para la solucion, recalcular los costos
    update_solution_costs();
}

unsigned int Solution::get_current_assignment(unsigned int process_id) {
    return assignments[process_id];
}