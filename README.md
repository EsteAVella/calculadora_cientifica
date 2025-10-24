# estructuras.h

Este archivo define las estructuras de datos utilizadas en todo el programa.

## ecuacion_t
Esta es la estructura principal del programa, y contiene toda la información de la ecuación.

**Campos:**
- `cadenaOriginal`: Almacena la ecuación original en formato texto
- `cantTokens`: Número total de tokens identificados
- `ecuacion`: Array con elementos del tipo "Token" de la ecuación en notación infija
- `cantTokensPostfijos`: Número de tokens en notación postfija
- `ecuacionPostfija`: Array con los tokens en notación postfija.

## Token
Representa cada elemento de la ecuación con su tipo y valor correspondiente. Utiliza una `union` para almacenar el valor, optimizando memoria ya que solo un campo estará activo a la vez.

**Campos:**
- `tipo`: Indica el tipo de token (DIGITO, OPERACION, INCOGNITA)
- `union`: Contiene el valor según el tipo:
  - `valor`: para DIGITO (entero)
  - `operacion`: para OPERACION (carácter)
  - `incognita`: para INCOGNITA (carácter)

## TipoToken
Enumeración que define los tipos posibles de tokens:

- `DIGITO` (0): Representa números
- `OPERACION` (1): Representa operadores matemáticos
- `INCOGNITA` (2): Representa variables/incógnitas


# reconocimiento.h

En este archivo se toma la cadena original de la ecuación, y se convierte cada carácter, en un tipo de dato manejable en operaciones.
### **`tokenizar`**
**Propósito:** Convierte una ecuación en texto a tokens estructurados.

**Parámetros:**

- `char *cadena`: Ecuación del usuario.
- `ecuacion_t *ecu`: Donde se guardan los tokens.

**Proceso:**

1. Guarda la cadena original.
2. Recorre carácter por carácter, creando un  dato del TipoToken para c/u:
    - **Dígitos:** Agrupa varios dígitos en un número (usa `atoi`)
    - **Operaciones/Incógnitas:** Crea un token individual por cada uno
3. Almacena cada token en `ecuacion[]` y actualiza `cantTokens`


### **`mostrarTokens`** y  **`mostrarPostfija`**
**Propósito**: Mostrar el resultado de la función **`tokenizar`** y **`aPostfijo`** (esta última se encuentra en otro archivo).

**Parámetros**: 
- `ecuacion_t *ecu`

**Proceso**:
1. Itera un numero de veces equivalente a la cantidad de Tokens que tenga la ecuación.
2. En cada iteración decide que tipo de Token es, y muestra en pantalla en base a esa información.



# dibujo.c

Este será el archivo principal. En este, se encuentra el menú principal, junto con todas las funciones correspondientes a los diferentes puntos del proyecto. 

## **`dibujarInicio`**
**Propósito:** Muestra las diferentes funcionalidades que tiene el programa. El usuario podrá elegir y, a través de un switch, `dibujarInicio` llamará  a la función correspondiente.

## `Funciones secundarias`
Funciones que se usarán a lo largo de todo el archivo, en diferentes funciones.
##### `limpiarConsola`:  Borra el contenido actual de la consola.
##### `pausa`: Mantiene el contenido de la consola, hasta que el usuario decida apretar enter.
##### `limpiarBufferEntrada`:  limpia cualquier carácter residual que haya quedado después de un ingreso.

## `escribirEcuacion`
**Propósito:** Permite al usuario ingresar una nueva ecuación mediante consola, verificando que sea válida antes de almacenarla y procesarla.

**Parámetros:**
- `ecuacion_t *ecuacion`: Puntero al array de ecuaciones.
- `int *posicion`: Posición del array donde se guardará la nueva ecuación.

**Proceso:**
1. Solicita al usuario que ingrese una ecuación por consola.
2. Verifica la validez de la ecuación usando `verificarEcuacionEscrita`.
3. Si es inválida, ofrece mostrar ayuda al usuario.
4. Permite editar la ecuación antes de confirmarla.
5. Almacena la ecuación validada en la posición indicada del arreglo.
6. Procesa la ecuación mediante tokenización y conversión a notación postfija.

**Resultado final:**  
La ecuación ingresada por el usuario queda validada, almacenada en el arreglo de ecuaciones en la posición especificada, y convertida a formato postfijo para su posterior cálculo.


## `verificarEcuacionEscrita`
**Propósito:** Valida que una ecuación ingresada contenga únicamente caracteres permitidos.

**Parámetros:**
- `char *ecuacion`: Cadena que contiene la ecuación a validar.

**Proceso:**
1. Recorre cada carácter de la ecuación uno por uno
2. Verifica que cada carácter sea correspondiente a uno permitido.
3. Si encuentra algún carácter no permitido, finaliza inmediatamente
4. Reemplaza el salto de línea por '\0'.

**Resultado final:**  
Retorna `'1'` si la ecuación contiene solo caracteres válidos, o `'0'` si se detecta algún carácter no permitido.

## `edicionEcuacion`
**Propósito:** Permite al usuario modificar una ecuación existente, validando que la nueva entrada sea correcta antes de reemplazarla.

**Parámetros:**
- `char* ecuacionAnterior`: Contiene la ecuación actual que será reemplazada.

**Proceso:**
1. Solicita al usuario que ingrese la nueva ecuación.
2. Si el usuario presiona Enter sin escribir nada, mantiene la ecuación original sin cambios.
3. Valida la nueva ecuación usando `verificarEcuacionEscrita`.
4. Repite el proceso hasta que se ingrese una ecuación válida.
5. Copia la nueva ecuación validada sobre la anterior.

**Resultado final:**  
La ecuación original es reemplazada por la nueva ecuación validada, o se mantiene sin cambios si el usuario no ingresa nada.


## `eliminarEcuacion`

**Propósito:** Elimina una ecuación específica del listado, mostrando todas las disponibles para que el usuario seleccione cuál borrar.

**Parámetros:**
- `ecuacion_t *ecuaciones`: Array con todas las ecuaciones.
- `int *cantEcuaciones`: Cantidad actual de ecuaciones.

**Proceso:**
1. Muestra todas las ecuaciones existentes usando `verEcuaciones`
2. Solicita al usuario el número de la ecuación a eliminar
3. Valida que la posición sea correcta
4. Desplaza todas las ecuaciones posteriores hacia arriba para llenar el espacio
5. Limpia los datos de la última posición que quedó duplicada
6. Reduce en 1 el contador total de ecuaciones

**Resultado final:**  
La ecuación seleccionada es eliminada del arreglo, las ecuaciones restantes se reordenan y el contador se actualiza.


## `verEcuaciones`

**Propósito:** Muestra en pantalla todas las ecuaciones almacenadas en el sistema.

**Parámetros:**
- `ecuacion_t *ecuacion`: Array de ecuaciones a mostrar.
- `int cantidadEcuaciones`: Cantidad total de ecuaciones cargadas.

**Proceso:**
1. Recorre cada ecuación en el arreglo.
2. Imprime cada ecuación con su número correspondiente (comenzando desde 1).
3. Utiliza el formato original en el que fue ingresada cada ecuación, es decir, una cadena de caracteres.

**Resultado final:**  
El usuario puede ver en pantalla un listado numerado de todas las ecuaciones disponibles en el sistema.

## `guardarReiniciar`

**Propósito:** Guarda todas las ecuaciones actuales en un archivo de texto y luego reinicia la sesión de trabajo actual.

**Parámetros:**
- `ecuacion_t *ecuacion`: Array de ecuaciones a guardar.
- `int *cant`: Cantidad de ecuaciones.

**Proceso:**
1. Genera un nombre de archivo único usando un contador.
2. Crea y abre un archivo de texto para escritura.
3. Guarda todas las ecuaciones en el archivo.
4. Cierra el archivo y confirma la operación.
5. Limpia el arreglo de ecuaciones actual.
6. Reinicia el contador de ecuaciones a 0.
7. Actualiza el contador de archivos para la próxima sesión.

**Resultado final:**  
Las ecuaciones se guardan en un archivo de texto con nombre único y la sesión actual queda vacía para comenzar una nueva.

## `leerEcuaciones`

**Propósito:** Permite al usuario seleccionar y cargar una de las sesiones guardadas en el sistema.

**Parámetros de entrada:**
- `ecuacion_t *ecuaciones`: Array donde se cargarán las ecuaciones leídas
- `int *cantEcuaciones`: Cantidad de ecuaciones cargadas.

**Proceso:**

1. Verifica si existen sesiones guardadas en el sistema.
2. Muestra un listado de todas las sesiones disponibles.
3. Solicita al usuario que seleccione una sesión por número.
4. Valida que la selección sea correcta.
5. Carga las ecuaciones del archivo seleccionado usando `abrirSesion`.

**Resultado final:**  
Las ecuaciones de la sesión seleccionada se cargan en el arreglo actual y se actualiza el contador de ecuaciones.

## `borrarEcuaciones`

**Propósito:** Elimina todos los archivos .txt de ecuaciones guardados en el sistema.

**Proceso:**
1. Solicita confirmación al usuario para proceder con la eliminación.
2. Si el usuario cancela, finaliza sin realizar cambios.
3. Recorre y elimina todos los archivos de ecuaciones (`ecuaciones-1.txt`, `ecuaciones-2.txt`, etc.)
4. Reinicia el contador de archivos a 1.
5. Informa la cantidad de archivos eliminados.

**Resultado final:**  
Todos los archivos de ecuaciones guardados son eliminados permanentemente y el sistema queda listo para comenzar una nueva numeración.


## `guardarContador`

**Propósito:** Guarda el valor actual del contador de sesiones en un archivo binario para mantener la numeración entre ejecuciones del programa.

**Parámetros:**
- `int contador`: Valor a guardar como contador actual.

**Proceso:**
1. Abre el archivo del contador en modo binario escritura.
2. Escribe el valor del contador en el archivo.
3. Cierra el archivo.
4. Maneja errores si no puede acceder al archivo.

**Resultado final:**  
El valor del contador queda guardado en el archivo, asegurando la numeración correcta para las próximas sesiones guardadas.


## `leerContador`

**Propósito:** Lee y gestiona el contador de archivos que se utiliza para numerar las sesiones guardadas.

**Proceso:**
1. Intenta abrir el archivo del contador en modo binario lectura
2. Si el archivo no existe, lo crea e inicializa el contador en 1
3. Lee el valor del contador desde el archivo
4. Valida que el contador esté dentro del rango permitido
5. Si está fuera de rango, lo restablece a 1

**Resultado final:**  
Retorna el número actual del contador (entre 1 y MAX_ARCHIVOS) para ser usado en la numeración de archivos.

## `abrirSesion`

**Propósito:** Carga en memoria las ecuaciones almacenadas en un archivo de sesión.

**Parámetros:**
- `ecuacion_t *ecuaciones`: Array donde se cargarán las ecuaciones leídas.
- `int numeroSesion`: Número que identifica la sesión a cargar.

**Proceso:**
1. Construye el nombre del archivo basado en el número de sesión.
2. Abre el archivo en modo lectura.
3. Lee línea por línea cada ecuación del archivo.
4. Elimina los saltos de línea de cada ecuación leída.
5. Almacena cada ecuación en el arreglo hasta alcanzar el límite máximo.

**Resultado final:**  
Retorna la cantidad de ecuaciones cargadas desde el archivo, o 0 si hubo error.

## `resolverEcuacion`

**Propósito:** Permite al usuario seleccionar una ecuación y elegir cómo resolverla: ingresando una tabla de valores completa o generándola a partir de un valor inicial.

**Parámetros:**
- `ecuacion_t* ecuaciones`: Array de ecuaciones disponibles.
- `int* cantEcuaciones`: Cantidad total de ecuaciones existentes.

**Proceso:**
1. Muestra todas las ecuaciones disponibles.
2. Solicita al usuario que seleccione una ecuación por número.
3. Presenta dos opciones de resolución:
    - Ingresar tabla de valores manualmente.
    - Generar tabla a partir de un valor.
4. Detecta si la ecuación contiene variables 'x' y/o 'y'.
5. Si se elige la opción 1, llama a `dibujarTabla` con los parámetros correspondientes.

**Resultado final:**  
Prepara la ecuación seleccionada para su resolución según el método elegido por el usuario.


## `dibujarTabla`

**Propósito:** Genera y muestra una tabla de resultados para una ecuación, permitiendo al usuario ingresar valores para las variables X e Y.

**Parámetros:**
- `bool tieneX`: Indica si la ecuación contiene la variable X.
- `bool tieneY`: Indica si la ecuación contiene la variable Y.
- `ecuacion_t* ecuacion`: Puntero a la ecuación que se va a resolver.

**Proceso:**
1. Solicita al usuario la cantidad de valores a evaluar (hasta MAX_VALORES).
2. Asigna memoria dinámica para almacenar los valores de X e Y según corresponda.
3. Permite al usuario ingresar los valores numéricos para cada variable.
4. Evalúa la ecuación para cada combinación de valores usando `resolverEcuacionEvaluada`.
5. Muestra una tabla formateada con los valores de entrada y resultados.
6. Libera la memoria asignada.

**Resultado final:**  
Se muestra en pantalla una tabla organizada con todos los valores ingresados y sus resultados correspondientes tras evaluar la ecuación.
