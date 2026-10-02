
Implementando un Grafo
=====


---

## IMPORTANTE 
### Antes de comenzar

1. Cree un repositorio en GitHub a partir del template proporcionado e impórtelo a su cuenta de Replit.
   Si no recuerda el procedimiento, revise el [tutorial](https://chartreuse-goal-d5c.notion.site/Instrucciones-para-realizar-los-Labs-301d965dc59e8034b554e1e6a9772615?source=copy_link).

2. El laboratorio **debe desarrollarse íntegramente en su cuenta personal de Replit**, **con el autocompletado basado en IA desactivado**.

   Vea [**AQUI**](https://chartreuse-goal-d5c.notion.site/Como-desactivar-llenado-autom-tico-de-repl-it-31ad965dc59e80b7b05bd02ae7970fdb?source=copy_link) como desactivar el autocompletado.

3. Solo está permitido modificar el archivo `graph.c` y no está permitido utilizar comandos Git.

4. **No está permitido copiar bloques de código de fuentes externas**.

5. Para ejecutar los tests, abra la terminal (Shell) en Replit y ejecute:

   ```bash
   bash test.sh
   ```

6. Debe ejecutar los tests **luego de terminar cada ejercicio**.

7. Al finalizar, adjunte en el aula virtual:

   * La URL de su repositorio GitHub (con los cambios actualizados).
   * El *join link* de su proyecto en Replit.
   * Si utilizó IA como herramienta de apoyo conceptual: enlace a la conversación completa **en un único chat**.

### 🤖 Sobre el uso de IA generativa

Se permite el uso de IA generativa únicamente como apoyo conceptual.

Está permitido:

* Solicitar explicaciones sobre conceptos del lenguaje C.
* Consultar el significado de errores o advertencias del compilador.
* Pedir orientación general **sin solicitar código**.

No está permitido:

* Pedir la solución completa o parcial de los ejercicios.
* Solicitar generación, corrección o reescritura del código del laboratorio.
* **Copiar o utilizar código generado por IA**.

Si utiliza herramientas de IA durante el laboratorio, debe:

* **Mantener la conversación completa en un único chat**.
* Adjuntar el enlace o la transcripción completa de dicha conversación junto con la entrega a través del aula virtual.

**Ejemplo de uso permitido (caso límite):**

Un estudiante puede compartir un fragmento de su propio código junto con el mensaje de error y preguntar:

> “Tengo este código y aparece un `segmentation fault`. ¿Qué causas comunes podrían producir este error? **No me des la solución ni el código corregido; solo explícame en palabras qué podría estar fallando.**”

---

# ACTIVIDAD: Implementación de un Grafo con Listas de Adyacencia

En este laboratorio implementaremos un **Grafo Dirigido** utilizando la representación de listas de adyacencia. Para lograr un acceso rápido a los nodos, combinaremos esta representación con un **Mapa (Diccionario)**.

Para ello usaremos las siguientes estructuras subyacentes:

```c
// Representa una conexión entre dos nodos
typedef struct Edge {
    char* target;   // Nombre del nodo destino
    int weight;     // Peso o costo de la arista
} Edge;

// Estructura principal del grafo
struct Graph {
    // Mapa: Llave (char* label) -> Valor (List* de Edge*)
    Map* adjacencyMap; 
};

```

Como se pueden dar cuenta, el grafo utiliza un solo mapa (`adjacencyMap`). Las **claves** de este mapa son los nombres de los nodos (cadenas de texto o `char*`), y los **valores** asociados a cada clave son una **Lista** que contiene elementos de tipo `Edge*` (las aristas que salen de ese nodo).

Dado que el mapa necesita saber cómo comparar las claves (que en este caso son strings), se proporciona la siguiente función auxiliar:

```c
// Función auxiliar para comparar strings en el mapa
int is_equal_string(void *key1, void *key2) {
    return strcmp((char*)key1, (char*)key2) == 0;
}

```

En las pruebas de código, se inicializará un mapa internamente pasando funciones de comparación adecuadas para strings. Su tarea es completar las operaciones fundamentales del grafo.

---

### Ejercicios

Revise el código base proporcionado en `graph.c`. También revise las operaciones de los TDA Lista y Mapa: `list.h`, `map.h`.

Debe completar la implementación de las siguientes funciones. Recuerde manejar correctamente la memoria y verificar que los parámetros no sean nulos (`NULL`).

**1.- Implemente la función `createGraph`**
Esta función debe crear y retornar un nuevo grafo vacío. Debe reservar memoria para la estructura `Graph` y luego inicializar su mapa interno (`adjacencyMap`) utilizando la función constructora del mapa (ej. `map_create(is_equal_string)`).

```c
Graph* createGraph() {
    // 1. Reserve memoria para el Grafo
    // 2. Inicialice g->adjacencyMap
    // 3. Retorne el grafo
    return NULL;
}

```

**2.- Implemente la función `addNode(Graph* g, const char* label)`**
Esta función agrega un nuevo vértice al grafo.

* Primero, debe verificar si el nodo ya existe en el `adjacencyMap`. Si ya existe, la función no debe hacer nada.
* Si no existe, debe crear una copia del string `label` (usando `strdup` o `malloc` + `strcpy`), crear una **nueva lista vacía** (que almacenará sus futuras aristas) e insertar el par `(copia_label, nueva_lista)` en el mapa.

```c
void addNode(Graph* g, const char* label) {
    if (!g || !label) return;

    // Su implementación aquí
}

```

**3.- Implemente la función `addEdge(Graph* g, const char* src, const char* dest, int weight)`**
Esta función agrega una arista dirigida desde el nodo `src` hacia el nodo `dest` con un peso `weight`.

* Debe buscar en el mapa la lista de aristas asociada al nodo origen (`src`). Si el nodo origen no existe, la función termina.
* Si existe, debe reservar memoria para una nueva estructura `Edge`.
* A este nuevo `Edge`, asígnele el `weight` y una **copia** del string `dest` (en el campo `target`).
* Finalmente, agregue este nuevo `Edge` a la lista de adyacencia del nodo `src`.

```c
void addEdge(Graph* g, const char* src, const char* dest, int weight) {
    if (!g || !src || !dest) return;

    // Su implementación aquí
}

```

**4.- Implemente la función `getEdges(Graph* g, const char* label)`**
Busca un nodo en el mapa a partir de su `label` y retorna un puntero a la Lista de aristas (`Edge*`) que salen de él. Si el nodo no existe en el grafo, debe retornar `NULL`.

```c
List* getEdges(Graph* g, const char* label) {
    if (!g || !label) return NULL;

    // Su implementación aquí
    return NULL;
}

```

**5.- Implemente la función `getWeight(Graph* g, const char* label1, const char* label2)`**
Obtiene el peso de la arista que conecta `label1` con `label2`.

* Debe obtener la lista de aristas de `label1`.
* Itere sobre esa lista. Por cada arista, compare su campo `target` con `label2`.
* Si coinciden, retorne el peso (`weight`) de esa arista.
* Si la lista se termina y no encontró el destino (o si el origen no existe), retorne `-1`.

```c
int getWeight(Graph* g, const char* label1, const char* label2) {
    if (!g || !label1 || !label2) return -1;

    // Su implementación aquí
    return -1; 
}

```

**6.- Implemente la función `getAdjacentLabels(Graph* g, const char* label)`**
Retorna una **nueva lista** que contenga únicamente los nombres (los strings) de los nodos adyacentes al nodo `label`.

* Obtenga la lista de aristas (estructuras `Edge*`) del nodo `label`.
* Cree una nueva Lista (`list_create()`).
* Itere sobre las aristas y agregue únicamente el campo `target` de cada arista a esta nueva lista.
* Retorne la nueva lista.

```c
List* getAdjacentLabels(Graph* g, const char* label) {
    if (!g || !label) return NULL;

    // Su implementación aquí
    return NULL; 
}

```
