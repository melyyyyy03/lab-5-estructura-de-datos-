#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "graph.c"



char * _strdup(const char * str) {
    char * aux = (char *)malloc(strlen(str) + 1);
    strcpy(aux, str);
    return aux;
}

char msg[500];
int test_id = -1;


int success(){
  printf("SUCCESS\n");
  exit(0);
}

void err_msg(char* msg){
    //if(test_id!=-1) return;
    printf("   [\033[31m FAILED \033[0m] ");
    printf("%s\n",msg);
    //print_trace();
}

void ok_msg(char* msg){
    //if(test_id!=-1) return;
    printf ("   [\033[32;1m OK \033[0m] ");
    printf("%s\n",msg);
}

void info_msg(char* msg){
    //if(test_id!=-1) return;
    printf ("   [ INFO ] %s\n",msg);
}

// Función auxiliar para inicializar mapas manualmente en los tests
int test_is_equal_string(void *key1, void *key2) {
    return strcmp((char*)key1, (char*)key2) == 0;
}

// ==========================================
//        PRUEBAS UNITARIAS AISLADAS
// ==========================================

int test_create() {
    info_msg("Probando createGraph de forma aislada...");

    Graph* g = createGraph();

    if (g == NULL) {
        err_msg("createGraph devolvio NULL.");
        return 0;
    }
    if (g->adjacencyMap == NULL) {
        err_msg("El mapa interno 'adjacencyMap' no fue inicializado.");
        return 0;
    }

    ok_msg("Grafo y mapa interno creados correctamente.");
    return 5;
}

int test_addNode() {
    info_msg("Probando addNode...");

    // 1. Preparamos el entorno manualmente
    Graph mock_graph;
    mock_graph.adjacencyMap = map_create(test_is_equal_string);

    // 2. Llamamos a la función a evaluar
    addNode(&mock_graph, "Santiago");

    // 3. Verificamos revisando directamente el mapa (sin usar getEdges)
    MapPair* pair = map_search(mock_graph.adjacencyMap, "Santiago");
    if (pair == NULL) {
        err_msg("El nodo 'Santiago' no fue insertado en el mapa.");
        return 0;
    }

    if (pair->value == NULL) {
        err_msg("Se insertó la llave, pero no se inicializó la lista (value == NULL).");
        return 0;
    }

    ok_msg("addNode insertó el nodo y creó su lista correctamente.");
    return 5;
}

int test_addEdge() {
    info_msg("Probando addEdge (inyectando nodos manualmente)...");

    // 1. Entorno manual
    Graph mock_graph;
    mock_graph.adjacencyMap = map_create(test_is_equal_string);

    List* lista_origen = list_create();
    List* lista_destino = list_create(); // Para que el destino exista

    map_insert(mock_graph.adjacencyMap, "A", lista_origen);
    map_insert(mock_graph.adjacencyMap, "B", lista_destino);

    // 2. Función a evaluar
    addEdge(&mock_graph, "A", "B", 15);

    // 3. Verificación directa en la lista manual
    Edge* e = (Edge*)list_first(lista_origen);
    if (e == NULL) {
        err_msg("No se insertó la arista en la lista de adyacencia de 'A'.");
        return 0;
    }

    if (strcmp(e->target, "B") != 0 || e->weight != 15) {
        err_msg("La arista insertada tiene un destino o peso incorrecto.");
        return 0;
    }

    ok_msg("addEdge funciona correctamente y de forma aislada.");
    return 10;
}

int test_getEdges() {
    info_msg("Probando getEdges (con datos precargados manualmente)...");

    Graph mock_graph;
    mock_graph.adjacencyMap = map_create(test_is_equal_string);

    // Creamos una lista falsa y la inyectamos en el mapa
    List* mock_list = list_create();
    map_insert(mock_graph.adjacencyMap, "Centro", mock_list);

    // Función a evaluar
    List* result = getEdges(&mock_graph, "Centro");

    if (result != mock_list) {
        err_msg("getEdges no retornó el puntero a la lista correcta.");
        return 0;
    }

    ok_msg("getEdges recupera la lista correctamente del mapa.");
    return 10;
}

int test_getWeight() {
    info_msg("Probando getWeight (con conexiones inyectadas)...");

    Graph mock_graph;
    mock_graph.adjacencyMap = map_create(test_is_equal_string);
    List* mock_list = list_create();

    // Simulamos que X apunta a Y con peso 42
    Edge* mock_edge = (Edge*)malloc(sizeof(Edge));
    mock_edge->target = _strdup("Y"); // Asumiendo que usas _strdup como en tus ejemplos previos
    mock_edge->weight = 42;
    list_pushBack(mock_list, mock_edge);

    map_insert(mock_graph.adjacencyMap, "X", mock_list);

    // Evaluación 1: Conexión que sí existe
    int w = getWeight(&mock_graph, "X", "Y");
    if (w != 42) {
        err_msg("El peso devuelto para una arista existente es incorrecto.");
        return 0;
    }

    // Evaluación 2: Conexión que no existe
    int w_inexistente = getWeight(&mock_graph, "Y", "X");
    if (w_inexistente != -1) {  // Asumiendo que tu función devuelve -1 al no encontrarla
        err_msg("Se esperaba que getWeight devolviera -1 si la arista no existe.");
        return 0;
    }

    ok_msg("getWeight funciona correctamente.");
    return 10;
}

// ==========================================
//        PRUEBA DE INTEGRACIÓN VERBOSA
// ==========================================

int test_integracion_rutas() {
    info_msg("Paso 1: Iniciando creacion del grafo de rutas...");
    Graph* g = createGraph();
    if (g == NULL) {
        err_msg("Fallo critico al intentar crear el grafo.");
        return 0;
    }
    ok_msg("Grafo inicializado en memoria.");

    info_msg("\nPaso 2: Registrando aeropuertos (Nodos)...");
    info_msg(" -> Agregando: Santiago, Lima, Bogota, Miami, Nueva York");
    addNode(g, "Santiago");
    addNode(g, "Lima");
    addNode(g, "Bogota");
    addNode(g, "Miami");
    addNode(g, "Nueva York");
    ok_msg("Aeropuertos registrados sin errores.");

    info_msg("\nPaso 3: Estableciendo rutas de vuelo y tiempos (Aristas)...");
    info_msg(" -> Conectando: Santiago -> Lima (230 min)");
    addEdge(g, "Santiago", "Lima", 230);

    info_msg(" -> Conectando: Santiago -> Bogota (350 min)");
    addEdge(g, "Santiago", "Bogota", 350);

    info_msg(" -> Conectando: Lima -> Bogota (180 min)");
    addEdge(g, "Lima", "Bogota", 180);

    info_msg(" -> Conectando: Lima -> Miami (340 min)");
    addEdge(g, "Lima", "Miami", 340);

    info_msg(" -> Conectando: Bogota -> Miami (210 min)");
    addEdge(g, "Bogota", "Miami", 210);

    info_msg(" -> Conectando: Miami -> Nueva York (180 min)");
    addEdge(g, "Miami", "Nueva York", 180);
    ok_msg("Todas las rutas fueron trazadas en el mapa.");

    info_msg("\nPaso 4: Verificando la integridad de los datos...");

    info_msg(" -> Validacion A: Cuantas rutas salen de Santiago? (Esperado: 2)");
    List* rutas_santiago = getEdges(g, "Santiago");
    if (list_size(rutas_santiago) != 2) {
        err_msg("Santiago no tiene las 2 rutas esperadas.");
        destroyGraph(g);
        return 0;
    }
    ok_msg("Validacion A superada: Santiago tiene exactamente 2 rutas.");

    info_msg(" -> Validacion B: Consultando tiempo de vuelo Lima -> Miami (Esperado: 340)");
    int tiempo_lima_miami = getWeight(g, "Lima", "Miami");
    if (tiempo_lima_miami != 340) {
        err_msg("El tiempo devuelto para Lima->Miami es incorrecto.");
        destroyGraph(g);
        return 0;
    }
    ok_msg("Validacion B superada: El tiempo es correcto.");

    info_msg(" -> Validacion C: Buscando conexion directa de Santiago -> Nueva York (Esperado: Nulo)");
    int vuelo_falso = getWeight(g, "Santiago", "Nueva York");
    if (vuelo_falso != -1) {
        err_msg("Se encontro una ruta directa que no deberia existir.");
        destroyGraph(g);
        return 0;
    }
    ok_msg("Validacion C superada: No existe ruta directa Santiago->Nueva York.");

    info_msg("\nPaso 5: Destruyendo el grafo para evitar fugas de memoria...");
    destroyGraph(g);
    ok_msg("Grafo destruido y memoria liberada exitosamente.");

    return 20; // 1 significa éxito
}

int test_suite(int(*test)(), char* msg, int max_score, int id, int req_id){
    if(test_id==-1 || test_id==id){
        printf("\n%s\n", msg);
        int score=test();

        if(id==req_id && score == max_score) success();

        printf("   partial_score: %d/%d\n", score, max_score); 

        return score;
    }
    return 1;
}


// ==========================================
//                  MAIN
// ==========================================

int main(int argc, char *argv[]) {
    int test_id = 0;
    if (argc > 1) test_id = atoi(argv[1]);
    srand(time(NULL));

    int total_score = 0;
    
    // Evaluamos las operaciones del Grafo.
    // Puntuacion total sugerida: 50 puntos (10 pts cada funcion)
    total_score += test_suite(test_create,    "Test Create Graph...", 5, 1, test_id);
    total_score += test_suite(test_addNode,   "Test Add Node...",     5, 2, test_id);
    total_score += test_suite(test_addEdge,   "Test Add Edge...",     10, 3, test_id);
    total_score += test_suite(test_getEdges,  "Test Get Edges...",    10, 4, test_id);
    total_score += test_suite(test_getWeight, "Test Get Weight...",   10, 5, test_id);
    total_score += test_suite(test_integracion_rutas, "Test Integrado...",   20, 6, test_id);
    
    
    if (argc == 1) {
        printf("\n========================================\n");
        printf("Puntaje total: %d/60\n", total_score);
        printf("========================================\n");
    }

    return 0;
}
