# LAB 2 - BD

## Integrantes:     
* José Sanchez Atahualpa
* Julisa Lapa Romero

## P1. Sequential File
``` cpp
    void insertAll(vector<Registro> registros);
    void add(Registro registro);
    Registro search(string key);
    vector<Registro> rangeSearch(string begin, string end);
```

## P2. ISAM - Sparse Index File
```cpp
    void add(Registro registro);
    vector<Registro> search(string key);
    vector<Registro> rangeSearch(string begin_key, string end_key);
```
## Análisis de accesos de memoria
### P1
```cpp
    void insertAll(vector<Registro> registros);
```

> Complejidad: O(1), sólo se realiza un acceso a memoria secundaria.


```cpp
    void add(Registro registro);
```

> Complejidad: O(log(n) + n + k), es decir, el costo del binary search más **n** que es el costo de llamar a la función reconstruct() y **k** que es el costo de acceder al archivo auxiliar.


```cpp
    Registro search(string key);
```

> Complejidad: O(log(n) + k), que sería el costo del binary search, más **k** que es el costo de retroceder hasta encontrar el primer registro con la key buscada y avanzar hasta el último.

```cpp
    vector<Registro> rangeSearch(string begin, string end);
```

> Complejidad: O(n + k), porque la búsqueda se hará hasta que se llegue al límite del rango establecido, que en el peor de los caso sería todo el archivo.

### P2
```cpp
    void add(Registro registro);
```

> Complejidad: O(k), donde **k** es la cantidad de páginas vinculadas a la primera pagina obtenida del archivo de datos.


```cpp
    vector<Registro> search(string key);
```

> Complejidad: O(k), donde **k** es la cantidad de páginas vinculadas a la primera pagina obtenida del archivo de datos.


```cpp
    vector<Registro> rangeSearch(string begin_key, string end_key);
```

> Complejidad: O(k), donde **k** es la cantidad de páginas vinculadas a la primera pagina obtenida del archivo de datos.