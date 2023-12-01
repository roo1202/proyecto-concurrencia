# Proyecto de Concurrencia

## Orientación

Realizar la implementación de la estructura de Lista Enlazada de enteros pero preparada para concurrencia. O sea, la estructura implementada debe ser capaz de poder realizar sus funciones de forma atómica sin importar la cantidad de operaciones concurrentes que sobre estas se realicen.

En el código entregado, las funciones a implementar se encuentran en los archivos [list.c](src/list.c), y [list.h](src/list.h) de la carpeta `src`. Estas son:

* `init_list`: Inicializa los datos necesarios para la estructura. Recibe un puntero a una lista a inicializar.
* `free_list`: Libera los datos utilizados por la estructura, incluido el puntero a la propia lista. Recibe un puntero a una lista.
* `size_list`: Retorna la cantidad de elementos almacenadas en la lista. Recibe un puntero a una lista.
* `index_list`: Retorna el elemento en un índice especificado. Recibe un puntero a una lista, el índice a obtener y un puntero a un entero que se utilizará para devolver el valor de dicho índice.
* `insert_list`: Inserta un elemento en un índice especificado. Recibe un puntero a una lista, un índice donde se insertará el elemento, y un entero el cual será insertado en la lista.
* `remove_list`: Elimina un elemento en un índice especificado. Recibe un puntero a una lista, un índice del elemento a eliminar, y un puntero a un valor con el elemento eliminado.

Adicionalmente cualquier dato adicional contenido dentro de la estructura de la lista debe ser agregado en el tipo definido dentro de [list.h](src/list.h).

### Especificaciones

1. Todas las funciones, con excepción de `size_list`, devuelven un entero el cual significa si la ejecución de dicha función fue exitosa o no.
2. El valor devuelto por las funciones `index_list` y `remove_list` de acuerdo a su lógica se realizará mediante el 3er argumento. En caso de ser vacía la lista estas deberán retornar 1.
3. Ninguna de las funciones que utilizan índices puede fallar por índices inválidos. Si el índice se encuentra fuera de los rangos actuales se considerará el mismo como si fuera el mayor valor que sí pertenezca al rango. O sea, 0 si el índice es negativo, la longitud menos 1 de la lista si el índice es mayor que la longitud en ese momento.
4. Todos los cambios en el proyecto deben limitarse a los archivos antes mencionados. No existe necesidad de realizar ningún cambio en otro lugar.

## Entrega

La entrega será realizada mediante Github a través de un Pull Request creado a este repositorio. Cualquier otra vía será considerada como inválida. El Pull Request creado debe contener Nombre Completo y Grupo.

## Evaluación

Al crear un Pull Request a este repositorio un Workflow de Github ejecutará el código implementado y dará una nota como resultado, además de los tiempos de ejecución del programa. Esta nota puede ser usada como referencia aunque la nota final del proyecto tendrá en cuenta también una revisión del código entregado. De forma adicional, los 10 mejores tiempos de ejecución obtendrán un punto adicional en la nota del proyecto.

## Ejecución Local

El proyecto incluye un archivo [Makefile](Makefile) que puede ser utilizado para compilar el proyecto. De igual forma, como parte del código entregado, se encuentra disponible una configuración de VSCode que puede ser utilizada para debugear el proyecto. Para el debugueo en VSCode es requerido una instalación funcional de las extensiones de [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack).
