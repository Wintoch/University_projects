#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Task{
    int id;
    double length;
}Task;

typedef struct TaskList{
    Task* tasks;
    int size;
    int taskCapacity;
    int totalTasks;
}TaskList;

void initTaskList(TaskList* tasklist, int numOfTasks){
    tasklist->size = 0;
    tasklist->taskCapacity = numOfTasks;
    tasklist->totalTasks = 0;
    tasklist->tasks = (Task*)malloc(numOfTasks * sizeof(Task));
}

void addTask(TaskList* tasklist, double length){
    tasklist->tasks[tasklist->size].id = tasklist->totalTasks + 1;
    tasklist->tasks[tasklist->size].length = length;
    tasklist->size += 1;
    tasklist->totalTasks += 1;
}

void FindOptimalC(TaskList* tasklist, int numOfProcessors, int step, double* processorTimes, double* bestCmax){
    if(step == tasklist->size) {
        double bestTime = 0;
        for (int i = 0; i < numOfProcessors; i++){
            if(processorTimes[i] > bestTime){
                bestTime = processorTimes[i];
            }
        }
        if(bestTime < *bestCmax){
            *bestCmax = bestTime;
        }
        return;
    };

    double currentLengthOfTask = tasklist->tasks[step].length;

    for (int i = 0; i < numOfProcessors; i++){
        if(processorTimes[i] + currentLengthOfTask < *bestCmax){
            processorTimes[i] += currentLengthOfTask;
            FindOptimalC(tasklist, numOfProcessors, step + 1, processorTimes, bestCmax);
            processorTimes[i] -= currentLengthOfTask;
        }
    }
}

void AlgorithmM(TaskList* tasklist, double* processors, int numOfProcessors, int numOfTasks, double* best){
    TaskList tempTaskList;
    tempTaskList.size = tasklist->size;
    tempTaskList.totalTasks = tasklist->totalTasks;
    tempTaskList.taskCapacity = tasklist->size;
    tempTaskList.tasks = (Task*)malloc(tasklist->size * sizeof(Task));
    for(int i = 0; i < tasklist->size; i++){
        tempTaskList.tasks[i] = tasklist->tasks[i];
    }

    double sumC = 0;

    int* task_counts = (int*)calloc(numOfProcessors, sizeof(int));
    int** assigned_ids = (int**)malloc(numOfProcessors * sizeof(int*));
    double** assigned_times = (double**)malloc(numOfProcessors * sizeof(double*));

    for(int i = 0; i < numOfProcessors; i++) {
        assigned_ids[i] = (int*)malloc(tasklist->size * sizeof(int));
        assigned_times[i] = (double*)malloc(tasklist->size * sizeof(double));
    }

    double capacity = 0;
    double cmaxlimit = 0;
    for (int i = 0; i < tempTaskList.size; i++){
        if (tempTaskList.tasks[i].length > cmaxlimit) cmaxlimit = tempTaskList.tasks[i].length;
        capacity += tempTaskList.tasks[i].length;
    }
    capacity /= numOfProcessors;
    if(capacity > cmaxlimit) cmaxlimit = capacity;
    double currentTime = 0;
    int currentTask = 0;
    int currentProcessor = 0;

    double eps = 1e-9;
    while(currentTask < tempTaskList.size){
        double space = cmaxlimit - currentTime;

        if (space < eps && currentProcessor < numOfProcessors - 1) {
            currentProcessor++;
            currentTime = 0.0;
            space = cmaxlimit;
        }

        if(tempTaskList.tasks[currentTask].length <= space + eps || currentProcessor == numOfProcessors - 1){
            processors[currentProcessor] += tempTaskList.tasks[currentTask].length;
            currentTime += tempTaskList.tasks[currentTask].length;

            int pos = task_counts[currentProcessor];
            assigned_ids[currentProcessor][pos] = tempTaskList.tasks[currentTask].id;
            assigned_times[currentProcessor][pos] = processors[currentProcessor];
            task_counts[currentProcessor]++;

            sumC += processors[currentProcessor];
            currentTask++;
        }else{
            double rest = tempTaskList.tasks[currentTask].length - space;

            processors[currentProcessor] += space;
            int pos = task_counts[currentProcessor];
            assigned_ids[currentProcessor][pos] = tempTaskList.tasks[currentTask].id;
            assigned_times[currentProcessor][pos] = processors[currentProcessor];
            task_counts[currentProcessor]++;
            
            sumC += processors[currentProcessor]; 

            currentProcessor++;
            currentTime = 0.0;

            processors[currentProcessor] += rest;
            pos = task_counts[currentProcessor];
            assigned_ids[currentProcessor][pos] = -tempTaskList.tasks[currentTask].id;
            assigned_times[currentProcessor][pos] = processors[currentProcessor];
            task_counts[currentProcessor]++;

            currentTime = processors[currentProcessor];
            currentTask++;
        }
    }
    *best = cmaxlimit;

    printf("Cmax: %g\n", *best);
    printf("sigmaC: %g\n", sumC);

    for (int i = 0; i < numOfProcessors; i++) {
        printf("M%d:", i + 1);
        for (int j = 0; j < task_counts[i]; j++) {
            if (assigned_ids[i][j] < 0) {
                printf("( P%d = %g )", -assigned_ids[i][j], assigned_times[i][j]);
            } else {
                printf("( C%d = %g )", assigned_ids[i][j], assigned_times[i][j]);
            }
        }
        printf("\n");
        free(assigned_ids[i]);
        free(assigned_times[i]);
    }

    free(assigned_ids);
    free(assigned_times);
    free(task_counts);
    free(tempTaskList.tasks);
}

void AlgorithmB(TaskList* tasklist, double* processors, int numOfProcessors){
    double sumC = 0;

    int* task_counts = (int*)calloc(numOfProcessors, sizeof(int));
    int** assigned_ids = (int**)malloc(numOfProcessors * sizeof(int*));
    double** assigned_times = (double**)malloc(numOfProcessors * sizeof(double*));

    for(int i = 0; i < numOfProcessors; i++) {
        assigned_ids[i] = (int*)malloc(tasklist->size * sizeof(int));
        assigned_times[i] = (double*)malloc(tasklist->size * sizeof(double));
    }

    for (int currentTask = 0; currentTask < tasklist->size; currentTask++) {
        int bestProcessorIndex = 0;
        for (int i = 1; i < numOfProcessors; i++) {
            if (processors[i] < processors[bestProcessorIndex])bestProcessorIndex = i;
        }

        processors[bestProcessorIndex] += tasklist->tasks[currentTask].length;
        
        sumC += processors[bestProcessorIndex];
        
        int pos = task_counts[bestProcessorIndex];
        assigned_ids[bestProcessorIndex][pos] = tasklist->tasks[currentTask].id;
        assigned_times[bestProcessorIndex][pos] = processors[bestProcessorIndex];
        
        task_counts[bestProcessorIndex]++;
    }

    double cmax = 0.0;
    for (int i = 0; i < numOfProcessors; i++) {
        if (processors[i] > cmax) cmax = processors[i];
    }

    printf("Cmax: %g\n", cmax);
    printf("sigmaC: %g\n", sumC);

    for (int i = 0; i < numOfProcessors; i++) {
        printf("M%d:", i + 1);
        for (int j = 0; j < task_counts[i]; j++) {
            printf("( C%d = %g )", assigned_ids[i][j], assigned_times[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < numOfProcessors; i++) {
        free(assigned_ids[i]);
        free(assigned_times[i]);
    }
    free(assigned_ids);
    free(assigned_times);
    free(task_counts);
}

void swapTasks(Task* a, Task* b) {
    Task temp = *a;
    *a = *b;
    *b = temp;
}

int partition(Task* tasks, int low, int high, bool ascending) {
    Task pivot = tasks[high];
    int i = (low - 1);

    if(ascending) {
        for (int j = low; j <= high - 1; j++) {
            if (tasks[j].length < pivot.length || (tasks[j].length == pivot.length && tasks[j].id < pivot.id)) {
                i++;
                swapTasks(&tasks[i], &tasks[j]);
            }
        }
    } else {
        for (int j = low; j <= high - 1; j++) {
            if (tasks[j].length > pivot.length || (tasks[j].length == pivot.length && tasks[j].id > pivot.id)) {
                i++;
                swapTasks(&tasks[i], &tasks[j]);
            }
        }
    }
    
    swapTasks(&tasks[i + 1], &tasks[high]);
    return (i + 1);
}

void QuickSort(Task* tasks, int low, int high, bool ascending) {
    if (low < high) {
        int pi = partition(tasks, low, high, ascending);
        QuickSort(tasks, low, pi - 1, ascending);
        QuickSort(tasks, pi + 1, high, ascending);
    }

}

void AlgorithmL(TaskList* tasklist, double* processors, int numOfProcessors) {
    TaskList tempTaskList;
    tempTaskList.size = tasklist->size;
    tempTaskList.totalTasks = tasklist->totalTasks;
    tempTaskList.taskCapacity = tasklist->size;
    tempTaskList.tasks = (Task*)malloc(tasklist->size * sizeof(Task));
    
    for (int i = 0; i < tasklist->size; i++) {
        tempTaskList.tasks[i] = tasklist->tasks[i];
    }
    
    QuickSort(tempTaskList.tasks, 0, tempTaskList.size - 1, false);
    AlgorithmB(&tempTaskList, processors, numOfProcessors);
    
    free(tempTaskList.tasks);
}

void AlgorithmS(TaskList* tasklist, double* processors, int numOfProcessors) {
    TaskList tempTaskList;
    tempTaskList.size = tasklist->size;
    tempTaskList.totalTasks = tasklist->totalTasks;
    tempTaskList.taskCapacity = tasklist->size;
    tempTaskList.tasks = (Task*)malloc(tasklist->size * sizeof(Task));
    
    for (int i = 0; i < tasklist->size; i++) {
        tempTaskList.tasks[i] = tasklist->tasks[i];
    }

    QuickSort(tempTaskList.tasks, 0, tempTaskList.size - 1, false);

    int* task_counts = (int*)calloc(numOfProcessors, sizeof(int));
    Task** buckets = (Task**)malloc(numOfProcessors * sizeof(Task*));
    for (int m = 0; m < numOfProcessors; m++) {
        buckets[m] = (Task*)malloc(tasklist->size * sizeof(Task));
    }

    for (int i = 0; i < tempTaskList.size; i++) {
        int m = i % numOfProcessors;
        buckets[m][task_counts[m]++] = tempTaskList.tasks[i];
    }

    for (int m = 0; m < numOfProcessors; m++) {
        if (task_counts[m] > 0) {
            QuickSort(buckets[m], 0, task_counts[m] - 1, true);
        }
    }

    double sumC = 0;
    double cmax = 0;

    double* machineEnd = (double*)calloc(numOfProcessors, sizeof(double));

     for (int m = 0; m < numOfProcessors; m++) {
        double t = 0.0;
        for (int j = 0; j < task_counts[m]; j++) {
            t += buckets[m][j].length;
            sumC += t;
        }
        if (t > cmax) cmax = t;
        machineEnd[m] = t;
    }

    printf("Cmax: %g\n", cmax);
    printf("sigmaC: %g\n", sumC);

    for (int m = 0; m < numOfProcessors; m++) {
        printf("M%d:", m + 1);
        double t = 0.0;
        for (int j = 0; j < task_counts[m]; j++) {
            t += buckets[m][j].length;
            printf("( C%d = %g )", buckets[m][j].id, t);
        }
        printf("\n");
    }

    free(machineEnd);
    for (int m = 0; m < numOfProcessors; m++) {
        free(buckets[m]);
    }
    free(buckets);
    free(task_counts);
    free(tempTaskList.tasks);
}


void AddTask(TaskList* tasks, int position, double length){
    if (tasks->size >= tasks->taskCapacity) {
        tasks->taskCapacity = (tasks->taskCapacity == 0) ? 10 : tasks->taskCapacity * 2; 
        tasks->tasks = (Task*)realloc(tasks->tasks, tasks->taskCapacity * sizeof(Task));
    }

    for (int i = tasks->size; i > position; i--) {
        tasks->tasks[i] = tasks->tasks[i - 1];
    }

    tasks->tasks[position].id = tasks->totalTasks + 1;
    tasks->tasks[position].length = length;
    
    tasks->size++;
    tasks->totalTasks++;
}

void RemoveTask(TaskList* tasks, int ID){
    for (int i = 0; i < tasks->size; i++){
        if(tasks->tasks[i].id == ID){
            for (int j = i; j < tasks->size - 1; j++){
                tasks->tasks[j] = tasks->tasks[j + 1];
            }
            tasks->size--;
            break;
        }
    }
}

int main(){
    int numOfTasks;
    scanf("%d", &numOfTasks);
    TaskList tasklist;
    initTaskList(&tasklist, numOfTasks);

    for (int i = 0; i < numOfTasks; i++){
        double lengthOfTask;
        scanf("%lf", &lengthOfTask);
        addTask(&tasklist, lengthOfTask);
    }

    int numOfProcessors;
    
    char operacja;
    while(scanf(" %c", &operacja)!= EOF) {

        if(operacja == 'A'){
            scanf("%d", &numOfProcessors);
            double bestCmax = 0.0;
            for (int i = 0; i < tasklist.size; i++){
                bestCmax += tasklist.tasks[i].length;
            }
            double* processorTimes = (double*)calloc(numOfProcessors, sizeof(double));
            FindOptimalC(&tasklist, numOfProcessors, 0, processorTimes, &bestCmax);
            printf("Copt: %g\n", bestCmax);
            free(processorTimes);
        }else if(operacja == 'M'){
            scanf("%d", &numOfProcessors);
            double bestCmax = 0.0;
            for (int i = 0; i < tasklist.size; i++){
                bestCmax += tasklist.tasks[i].length;
            }
            double* processors = (double*)calloc(numOfProcessors, sizeof(double));
            AlgorithmM(&tasklist, processors, numOfProcessors, numOfTasks, &bestCmax);
            free(processors);
        } else if (operacja == 'B'){
            scanf("%d", &numOfProcessors);
            double* processors = (double*)calloc(numOfProcessors, sizeof(double));
            AlgorithmB(&tasklist, processors, numOfProcessors);
            free(processors);
        }else if (operacja == 'L'){
            scanf("%d", &numOfProcessors);
            double* processors = (double*)calloc(numOfProcessors, sizeof(double));
            AlgorithmL(&tasklist, processors, numOfProcessors);
            free(processors);
        }else if (operacja == '-'){
            int ID;
            scanf("%d", &ID);
            RemoveTask(&tasklist, ID);
        }else if (operacja == '+'){
            int position;
            double length;
            scanf("%d %lf", &position, &length);
            position--;
            AddTask(&tasklist, position, length);
        }else if (operacja == 'S'){
            scanf("%d", &numOfProcessors);
            double* processors = (double*)calloc(numOfProcessors, sizeof(double));
            AlgorithmS(&tasklist, processors, numOfProcessors);
            free(processors);
        }
                
    }

    free(tasklist.tasks);
    return 0;
}