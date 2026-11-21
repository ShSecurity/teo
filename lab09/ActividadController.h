#pragma once

#include "CsvRepositorioAdapter.h"

class ActividadController {
public:
    ActividadController(RepositorioActividades& repo, const IRepositorio& fuente)
        : repo_(repo), fuente_(fuente) {}

    bool registrar(const QString& tipo, const QString& titulo, QDate fecha) {
        auto actividad = crearActividad(tipo, titulo, fecha);
        if (!actividad)
            return false;
        repo_.agregar(std::move(actividad));
        return true;
    }

    std::size_t cargar(const QString& ruta) {
        auto lote = fuente_.cargarTodo(ruta);
        const auto cantidad = lote.size();
        repo_.incorporar(std::move(lote));
        return cantidad;
    }

    const Actividades& listar() const { return repo_.listar(); }

private:
    RepositorioActividades& repo_;
    const IRepositorio& fuente_;
};
