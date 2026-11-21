#pragma once

#include <QString>
#include <QDate>
#include <QVector>

struct Actividad {
    QString tipo;
    QString titulo;
    QDate fecha;
};

class RepositorioActividades {
public:
    void agregar(Actividad actividad) { actividades_.append(actividad); }
    const QVector<Actividad>& listar() const { return actividades_; }

private:
    QVector<Actividad> actividades_;
};
