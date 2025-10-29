#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

// Tipos de elementos posibles que puede tener cada Token.
typedef enum{
    DIGITO, //0
    OPERACION, //1
    INCOGNITA //2
}TipoToken;

/*
    Esta estructura está pensada para formar separar CADA elemento de la
    cadena original, dandole un tipo de elemento posible, y guardandolo con
    su correspondiente espacio en memoria.

    union: En un struct todos los campos estan al mismo
        tiempo. O sea, hay un espacio de memoria para cada uno. En un "union",
        todos comparten el mismo espacio de memoria (se pisan), ya que esta
        pensado para que solo exista UNO a la vez.

    Posibles elementos de salida:
        DIGITO(56)= TipoToken=DIGITO; valor=56
*/

typedef struct{
    TipoToken tipo;
    union{
        int valor;
        char operacion;
        char incognita;
    };
}Token;

/*
    Acá es donde se terminan aplicando la estructura anterior.
    Primero, se va a guardar la cadenaOriginal, para poder despues guardarla
    en el txt, y poder mostrarla con mayor facilidad.

    Despues, ecuacion[] sera un array, el cual contendra el contenido de cada
    byte de la cadena, pero ya clasificado y transformado.

    cantTokens te dice la cantidad de elementos en el array ecuacion.
*/

typedef struct{
    char cadenaOriginal[50];
    int cantTokens;
    Token ecuacion[50];
    int cantTokensPostfijos;
    Token ecuacionPostfija[50];
}ecuacion_t;



#endif // ESTRUCTURAS_H
