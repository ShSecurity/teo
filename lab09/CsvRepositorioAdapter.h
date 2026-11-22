#pragma once

#include "ActividadFactory.h"
#include "IRepositorio.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <stdexcept>

class FuenteCsvHeredada {
public:
    QStringList obtenerRegistrosCrudos(const QString& ruta) const {
        QFile archivo(ruta);
        if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text))
            throw std::runtime_error(archivo.errorString().toStdString());
        QTextStream entrada(&archivo);
        QStringList registros;
        while (!entrada.atEnd())
            registros.append(entrada.readLine());
        if (entrada.status() != QTextStream::Ok)
            throw std::runtime_error("Error al leer el CSV.");
        return registros;
    }
};

class CsvRepositorioAdapter final : public IRepositorio {
public:
    Actividades cargarTodo(const QString& ruta) const override {
        Actividades lote;
        const auto registros = fuente_.obtenerRegistrosCrudos(ruta);
        for (qsizetype i = 0; i < registros.size(); ++i) {
            const QString linea = registros[i].trimmed();
            if (linea.isEmpty() || (i == 0 && linea.compare("tipo;titulo;fecha", Qt::CaseInsensitive) == 0))
                continue;
            const auto campos = linea.split(';');
            if (campos.size() == 3) {
                auto actividad = crearActividad(campos[0], campos[1],
                    QDate::fromString(campos[2].trimmed(), Qt::ISODate));
                if (actividad) {
                    lote.push_back(std::move(actividad));
                    continue;
                }
            }
            throw std::runtime_error(QString("Registro inválido en la línea %1.").arg(i + 1).toStdString());
        }
        return lote;
    }

private:
    FuenteCsvHeredada fuente_;
};
