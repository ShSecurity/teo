#pragma once

#include <QDate>
#include <QString>
#include <memory>
#include <utility>
#include <vector>

class Actividad {
public:
    Actividad(QString titulo, QDate fecha) : titulo_(std::move(titulo)), fecha_(fecha) {}
    virtual ~Actividad() = default;
    const QString& titulo() const { return titulo_; }
    QDate fecha() const { return fecha_; }
    virtual QString tipo() const = 0;

private:
    QString titulo_;
    QDate fecha_;
};

class Tarea final : public Actividad {
public:
    using Actividad::Actividad;
    QString tipo() const override { return "Tarea"; }
};

class Examen final : public Actividad {
public:
    using Actividad::Actividad;
    QString tipo() const override { return "Examen"; }
};

class Proyecto final : public Actividad {
public:
    using Actividad::Actividad;
    QString tipo() const override { return "Proyecto"; }
};

using Actividades = std::vector<std::unique_ptr<Actividad>>;

class RepositorioActividades {
public:
    void agregar(std::unique_ptr<Actividad> actividad) {
        actividades_.push_back(std::move(actividad));
    }

    void incorporar(Actividades lote) {
        actividades_.reserve(actividades_.size() + lote.size());
        for (auto& actividad : lote)
            agregar(std::move(actividad));
    }

    const Actividades& listar() const { return actividades_; }

private:
    Actividades actividades_;
};
