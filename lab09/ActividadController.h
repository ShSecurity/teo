#pragma once

#include "Actividad.h"

class ActividadController {
public:
    explicit ActividadController(RepositorioActividades& repo) : repo_(repo) {}

    bool registrar(const QString& tipo, const QString& titulo, QDate fecha) {
        if (titulo.trimmed().isEmpty() || !fecha.isValid())
            return false;
        repo_.agregar({tipo, titulo.trimmed(), fecha});
        return true;
    }

    const QVector<Actividad>& listar() const { return repo_.listar(); }

private:
    RepositorioActividades& repo_;
};
