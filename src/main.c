#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#include "list.h"

#define MAX_NUM 100
// General data
float base_grade = 2;
float correct_init_size_free = 0.20;
float correct_result = 1.0;
// Insertions data
const int INS_GEN_THREADS = 200;
const int INS_GEN_MAX_OPS = 4000;
float correct_insert = 0.65;
int effective_inserts = 0;
int effective_inserts_per_value[MAX_NUM + 1];
// Removals data
const int REM_GEN_THREADS = 200;
const int REM_GEN_MAX_OPS = 1000;
float correct_remove = 0.65;
int effective_removals = 0;
int effective_removals_per_value[MAX_NUM + 1];
// Indexs data
const int IND_GEN_THREADS = 300;
const int IND_GEN_MAX_OPS = 2000;
float correct_index = 0.5;
// Results data
int results_cant[MAX_NUM + 1];
struct timespec begin, end;

int_ll_t *the_list;

// Generators

//// Insertions
typedef struct _insert_worker_input_t
{
    int value, times;
} insert_worker_input_t;

void *insert_worker(void *arg)
{
    insert_worker_input_t *input = (insert_worker_input_t *)arg;
    for (int i = 0; i < input->times; ++i)
    {
        int size = size_list(the_list);
        int pos = rand() % (size + 1);
        if (insert_list(the_list, pos, input->value) != 0)
        {
            correct_insert = 0;
        }
    }
    return NULL;
}

void *start_inserts(void *arg)
{
    int ops_done = 0;
    int ops_per_thread = INS_GEN_MAX_OPS / INS_GEN_THREADS;
    insert_worker_input_t inputs[INS_GEN_THREADS];
    pthread_t threads[INS_GEN_THREADS];
    for (int i = 0; i < INS_GEN_THREADS; ++i)
    {
        insert_worker_input_t *input = &inputs[i];
        input->value = rand() % (MAX_NUM + 1);
        if (ops_done + ops_per_thread > INS_GEN_MAX_OPS)
        {
            input->times = INS_GEN_MAX_OPS - ops_done;
        }
        else
        {
            input->times = ops_per_thread;
        }
        ops_done += input->times;
        if (pthread_create(&threads[i], NULL, insert_worker, input) != 0)
        {
            printf("Error al crear el hilo de inserción %d!\n", i);
            exit(1);
        }
    }
    for (int i = 0; i < INS_GEN_THREADS; ++i)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            printf("Error al esperar por el hilo de inserción %d!\n", i);
            exit(1);
        }
        effective_inserts += inputs[i].times;
        effective_inserts_per_value[inputs[i].value] += inputs[i].times;
    }
    return NULL;
}

//// Removals

typedef struct _remove_worker_input_t
{
    int times;
} remove_worker_input_t;

typedef struct _remove_worker_output_t
{
    int *values;
    int effective_times;
} remove_worker_output_t;

void *remove_worker(void *arg)
{
    remove_worker_input_t *input = (remove_worker_input_t *)arg;
    remove_worker_output_t *output = malloc(sizeof(remove_worker_output_t));
    output->effective_times = 0;
    output->values = malloc(sizeof(int) * input->times);
    for (int i = 0; i < input->times; ++i)
    {
        int size = size_list(the_list);
        int pos = rand() % (size + 1);
        int value;
        if (remove_list(the_list, pos, &value) == 0)
        {
            output->values[output->effective_times] = value;
            ++output->effective_times;
        }
    }
    return output;
}

void *start_removals(void *arg)
{
    int ops_done = 0;
    int ops_per_thread = REM_GEN_MAX_OPS / REM_GEN_THREADS;
    remove_worker_input_t inputs[REM_GEN_THREADS];
    pthread_t threads[REM_GEN_THREADS];
    for (int i = 0; i < REM_GEN_THREADS; ++i)
    {
        remove_worker_input_t *input = &inputs[i];
        if (ops_done + ops_per_thread > REM_GEN_MAX_OPS)
        {
            input->times = REM_GEN_MAX_OPS - ops_done;
        }
        else
        {
            input->times = ops_per_thread;
        }
        ops_done += input->times;
        if (pthread_create(&threads[i], NULL, remove_worker, input) != 0)
        {
            printf("Error al crear el hilo de eliminación %d!\n", i);
            exit(1);
        }
    }
    for (int i = 0; i < REM_GEN_THREADS; ++i)
    {
        remove_worker_output_t *output;
        if (pthread_join(threads[i], (void **)&output) != 0)
        {
            printf("Error al esperar por el hilo de eliminación %d!\n", i);
            exit(1);
        }
        effective_removals += output->effective_times;
        for (int j = 0; j < output->effective_times; ++j)
        {
            int value = output->values[j];
            ++effective_removals_per_value[value];
        }
        free(output->values);
        free(output);
    }
    return NULL;
}

//// Indexs

typedef struct _index_worker_input_t
{
    int times;
} index_worker_input_t;

void *index_worker(void *arg)
{
    index_worker_input_t *input = (index_worker_input_t *)arg;
    for (int i = 0; i < input->times; ++i)
    {
        int size = size_list(the_list);
        int pos = rand() % (size + 1);
        int value;
        if (index_list(the_list, pos, &value) != 0)
        {
            correct_index = 0;
        }
    }
    return NULL;
}

void *start_indexs(void *arg)
{
    int ops_done = 0;
    int ops_per_thread = IND_GEN_MAX_OPS / IND_GEN_THREADS;
    index_worker_input_t inputs[IND_GEN_THREADS];
    pthread_t threads[IND_GEN_THREADS];
    for (int i = 0; i < IND_GEN_THREADS; ++i)
    {
        index_worker_input_t *input = &inputs[i];
        if (ops_done + ops_per_thread > IND_GEN_MAX_OPS)
        {
            input->times = IND_GEN_MAX_OPS - ops_done;
        }
        else
        {
            input->times = ops_per_thread;
        }
        ops_done += input->times;
        if (pthread_create(&threads[i], NULL, index_worker, input) != 0)
        {
            printf("Error al crear el hilo de indexación %d!\n", i);
            exit(1);
        }
    }
    for (int i = 0; i < IND_GEN_THREADS; ++i)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            printf("Error al esperar por el hilo de indexación %d!\n", i);
            exit(1);
        }
    }
    return NULL;
}

// Starter
void *
start_generators(void *arg)
{
    // Start insertions
    pthread_t inserts_starter;
    if (pthread_create(&inserts_starter, NULL, start_inserts, NULL) != 0)
    {
        printf("Error al crear el hilo de lanzamiento de inserciones!\n");
        exit(1);
    }
    // Start removals
    pthread_t removals_starter;
    if (pthread_create(&removals_starter, NULL, start_removals, NULL) != 0)
    {
        printf("Error al crear el hilo de lanzamiento de eliminaciones!\n");
        exit(1);
    }
    // Start indexs
    pthread_t indexs_starter;
    if (pthread_create(&indexs_starter, NULL, start_indexs, NULL) != 0)
    {
        printf("Error al crear el hilo de lanzamiento de indexaciones!\n");
        exit(1);
    }
    // Wait for insertions to finish
    if (pthread_join(inserts_starter, NULL) != 0)
    {
        printf("Error al esperar por el hilo de lanzamiento de inserciones!\n");
        exit(1);
    }
    // Wait for removals to finish
    if (pthread_join(removals_starter, NULL) != 0)
    {
        printf("Error al esperar por el hilo de lanzamiento de eliminaciones!\n");
        exit(1);
    }
    // Wait for indexs to finish
    if (pthread_join(indexs_starter, NULL) != 0)
    {
        printf("Error al esperar por el hilo de lanzamiento de indexaciones!\n");
        exit(1);
    }
    return NULL;
}

// Main
int main(int argc, char **argv)
{
    // Init list
    the_list = malloc(sizeof(int_ll_t));
    // Test init
    if (init_list(the_list) != 0)
    {
        printf("Error al inicializar la lista!\n");
        correct_init_size_free = 0;
    }
    // Launch generators
    pthread_t generators_starter;
    clock_gettime(CLOCK_MONOTONIC_RAW, &begin);
    if (pthread_create(&generators_starter, NULL, start_generators, NULL) != 0)
    {
        printf("Error al crear el hilo de lanzamiento de generadores!\n");
        exit(1);
    }

    // Wait for generators to finish
    if (pthread_join(generators_starter, NULL) != 0)
    {
        printf("Error al esperar por el hilo de lanzamiento de generadores!\n");
        exit(1);
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    // Test size
    int expected_size = effective_inserts - effective_removals;
    int size_result = size_list(the_list);
    if (size_result <= 0 || size_result != expected_size)
    {
        printf("Tamaño de la lista incorrecto! Esperado %d, obtenido %d\n", expected_size, size_result);
        correct_init_size_free = 0;
    }

    // Verify results
    if (effective_inserts != INS_GEN_MAX_OPS)
    {
        printf("Cantidad de inserciones insuficiente! Esperado %d, obtenido %d\n", INS_GEN_MAX_OPS, effective_inserts);
        correct_insert = 0;
    }
    for (int i = 0; i < size_result; ++i)
    {
        int value;
        if (index_list(the_list, i, &value) != 0)
        {
            printf("Error al obtener el elemento en la posición %d!\n", i);
            correct_index = 0;
        }
        ++results_cant[value];
    }
    for (int n = 0; n <= MAX_NUM; ++n)
    {
        int n_inserts = effective_inserts_per_value[n];
        int n_removes = effective_removals_per_value[n];
        // Check that removals are correct
        if (n_inserts < n_removes)
        {
            printf("Cantidad de eliminaciones inválidas del valor %d! Inserciones: %d y Eliminaciones: %d\n", n, n_inserts, n_removes);
            correct_remove = 0;
            correct_insert = 0;
        }
        // Check that results are correct
        if (results_cant[n] != n_inserts - n_removes)
        {
            printf("Cantidad de resultados inválida del valor %d! Esperado %d, obtenido %d\n", n, n_inserts - n_removes, results_cant[n]);
            correct_index = 0;
            correct_result = 0;
        }
    }

    // Test free
    if (free_list(the_list) != 0)
    {
        printf("Error al liberar la lista!\n");
        correct_init_size_free = 0;
    }

    // Print grade
    float grade = base_grade + correct_init_size_free + correct_insert + correct_remove + correct_index + correct_result;
    printf("\n\nNota: %.2f\n", grade);
    // Print time taken
    float time_taken = ((end.tv_nsec - begin.tv_nsec) / 1000000000.0) + (end.tv_sec - begin.tv_sec);
    printf("Tiempo: %f segundos\n", time_taken);
    if (grade < 3.0)
    {
        exit(1);
    }
    exit(0);
}
