#pragma once

#include "Actividad.h"

class IRepositorio {
public:
    virtual ~IRepositorio() = default;
    virtual Actividades cargarTodo(const QString& ruta) const = 0;
};
