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
