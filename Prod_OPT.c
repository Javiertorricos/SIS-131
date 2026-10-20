#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

#define SHM_NAME "/bus_shm_opt"
#define FILE_NAME "paquetes_disco.bin"
#define NUM_PACKETS 10000000
#define PKT_SIZE 64
#define BATCH_SIZE 500
#define TOTAL_BATCHES (NUM_PACKETS / BATCH_SIZE)

typedef struct {
    int id;
    char payload[PKT_SIZE - sizeof(int)];
} Packet;

typedef struct {
    sem_t sem_read;
    sem_t sem_write;
    Packet batch[BATCH_SIZE];
} SharedBusBuffer;

int main() {
    // --- 1. GESTIÓN DE MEMORIA SECUNDARIA (DISCO) ---
    FILE *file = fopen(FILE_NAME, "rb");
    if (!file) {
        printf("[Prod_OPT] El archivo en disco no existe. Creando '%s' con %d paquetes...\n", FILE_NAME, NUM_PACKETS);
        file = fopen(FILE_NAME, "wb+");
        if (!file) {
            perror("Error al crear el archivo en disco");
            return 1;
        }
        
        Packet temp_pkt;
        for (int i = 0; i < NUM_PACKETS; i++) {
            temp_pkt.id = i;
            memset(temp_pkt.payload, 'B', sizeof(temp_pkt.payload));
            fwrite(&temp_pkt, sizeof(Packet), 1, file);
        }
        rewind(file); // Regresar al inicio para la lectura
    } else {
        printf("[Prod_OPT] Leyendo paquetes desde el archivo en disco existente ('%s')...\n", FILE_NAME);
    }

    // --- 2. CONFIGURACIÓN DE MEMORIA COMPARTIDA POSIX (mmap) ---
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Error en shm_open");
        fclose(file);
        return 1;
    }
    
    // Validación correcta de ftruncate para evitar advertencias de compilación (-Wunused-result)
    if (ftruncate(shm_fd, sizeof(SharedBusBuffer)) == -1) {
        perror("Error en ftruncate");
        close(shm_fd);
        fclose(file);
        return 1;
    }
    
    SharedBusBuffer *shm = mmap(0, sizeof(SharedBusBuffer), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm == MAP_FAILED) {
        perror("Error en mmap");
        close(shm_fd);
        fclose(file);
        return 1;
    }

    // Inicializar semáforos POSIX
    sem_init(&shm->sem_read, 1, 0);
    sem_init(&shm->sem_write, 1, 1);

    printf("[Prod_OPT] Iniciando transferencia optimizada (Disco -> Shared Memory con Batching)...\n");

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // --- 3. BUCLE DE TRANSFERENCIA POR LOTES (BATCHING) ---
    for (int b = 0; b < TOTAL_BATCHES; b++) {
        sem_wait(&shm->sem_write);

        // Lectura directa desde disco hacia la memoria compartida
        size_t read_count = fread(shm->batch, sizeof(Packet), BATCH_SIZE, file);
        if (read_count <= 0) {
            break;
        }

        sem_post(&shm->sem_read);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    double pps = NUM_PACKETS / elapsed;
    double mbps = (NUM_PACKETS * PKT_SIZE) / (1024.0 * 1024.0 * elapsed);

    // --- 4. MÉTRICAS Y RESULTADOS ---
    printf("\n=== METRICAS PRODUCTOR OPTIMIZADO (Prod_OPT con Lectura de Disco) ===\n");
    printf("Paquetes enviados : %d\n", NUM_PACKETS);
    printf("Tiempo total      : %.4f segundos\n", elapsed);
    printf("Velocidad (PPS)   : %.0f paquetes/seg\n", pps);
    printf("Rendimiento       : %.2f MB/s\n", mbps);

    // Limpieza de recursos
    fclose(file);
    munmap(shm, sizeof(SharedBusBuffer));
    close(shm_fd);

    return 0;
}