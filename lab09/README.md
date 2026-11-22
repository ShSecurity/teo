# Seguimiento académico con Qt

Práctica 09 de Tecnología de Objetos. Registra tareas, exámenes y proyectos e importa actividades desde un CSV heredado.

## Compilar y probar

Requiere Qt 6 Widgets y Qt Test, CMake 3.20 o superior, Ninja y un compilador C++20 que coincida con el kit Qt. Configuración verificada en Windows con Qt 6.12.0, MinGW 13.1.0 y CMake 3.29.2. Ejecutar desde la raíz del repositorio en PowerShell y ajustar las rutas si Qt se instaló en otra ubicación.

```powershell
$env:Path = 'C:\Qt\6.12.0\mingw_64\bin;C:\Qt\Tools\mingw1310_64\bin;' + $env:Path
cmake -S lab09 -B build -G Ninja -DCMAKE_PREFIX_PATH=C:/Qt/6.12.0/mingw_64 -DCMAKE_CXX_COMPILER=C:/Qt/Tools/mingw1310_64/bin/g++.exe
cmake --build build
ctest --test-dir build --output-on-failure
.\build\seguimiento.exe
```

En Qt Creator, abrir el CMakeLists.txt de lab09 y seleccionar el kit Desktop Qt 6.12.0 MinGW 64-bit.

## Uso

- Seleccionar un tipo y una fecha, escribir el título y pulsar **Registrar actividad**. La lista se actualiza después de cada registro exitoso.
- Pulsar **Importar CSV** y abrir datos/actividades.csv. El formato esperado es UTF-8 con tres campos separados por punto y coma, fecha ISO y encabezado opcional tipo;titulo;fecha en la primera línea. Se omiten líneas vacías. Los campos usan texto simple sin escape de separadores.
- Un registro inválido cancela el lote completo y muestra la línea con error. La lista vive en memoria durante la sesión. Importar el mismo archivo otra vez agrega nuevamente sus actividades.

## Arquitectura y patrones

MainWindow emite solicitudes y renderiza la lista. ActividadController valida mediante las fábricas, coordina la importación y usa RepositorioActividades. El modelo utiliza Qt Core para QString y QDate y se prueba sin Qt Widgets.

Factory Method delega la construcción a TareaFactory, ExamenFactory y ProyectoFactory. La función crearActividad resuelve el tipo recibido y la fábrica base centraliza la validación. CsvRepositorioAdapter implementa IRepositorio y transforma las líneas que entrega FuenteCsvHeredada en actividades, sin acoplar al controlador con el formato CSV.

Los diagramas PlantUML están en diagramas/. El diagrama de secuencias documenta crear, listar y cargar.

## Comparación de dependencias

| Medida | Antes (escenario de la guía) | Después (código de este proyecto) |
|---|---:|---:|
| Clases de actividad concretas referenciadas por la vista | 3 | 0 |
| Lectores o adaptadores CSV referenciados por la vista | 1 | 0 |
| Archivos de lógica existentes que cambian al variar los separadores del CSV | 1 (vista) | 1 (adaptador) |
| Archivos de producción existentes que cambian al añadir un tipo seleccionable | 1 (vista) | 3 (productos, fábricas y opciones de la vista) |

El escenario anterior es una referencia de diseño de la guía. Las cifras del diseño actual cuentan dependencias directas por clase y archivos de producción afectados. La separación elimina de la vista la construcción y el análisis del CSV. Agregar un tipo cuesta más archivos en esta implementación, pero mantiene intactos el controlador, el repositorio y el adaptador. También hay que ampliar las pruebas para verificar el tipo nuevo.
