#pragma once

#include "Actividad.h"

class ActividadFactory {
public:
    virtual ~ActividadFactory() = default;

    std::unique_ptr<Actividad> crear(const QString& titulo, QDate fecha) const {
        if (titulo.trimmed().isEmpty() || !fecha.isValid())
            return nullptr;
        return construir(titulo.trimmed(), fecha);
    }

protected:
    virtual std::unique_ptr<Actividad> construir(const QString& titulo, QDate fecha) const = 0;
};

class TareaFactory final : public ActividadFactory {
    std::unique_ptr<Actividad> construir(const QString& titulo, QDate fecha) const override {
        return std::make_unique<Tarea>(titulo, fecha);
    }
};

class ExamenFactory final : public ActividadFactory {
    std::unique_ptr<Actividad> construir(const QString& titulo, QDate fecha) const override {
        return std::make_unique<Examen>(titulo, fecha);
    }
};

class ProyectoFactory final : public ActividadFactory {
    std::unique_ptr<Actividad> construir(const QString& titulo, QDate fecha) const override {
        return std::make_unique<Proyecto>(titulo, fecha);
    }
};

inline std::unique_ptr<Actividad> crearActividad(const QString& tipo, const QString& titulo, QDate fecha) {
    const QString clave = tipo.trimmed().toLower();
    if (clave == "tarea")
        return TareaFactory().crear(titulo, fecha);
    if (clave == "examen")
        return ExamenFactory().crear(titulo, fecha);
    if (clave == "proyecto")
        return ProyectoFactory().crear(titulo, fecha);
    return nullptr;
}
