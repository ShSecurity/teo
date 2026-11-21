#pragma once

#include "Actividad.h"
#include <QComboBox>
#include <QDateEdit>
#include <QFormLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow() {
        setWindowTitle("Seguimiento académico");
        resize(560, 380);
        auto* layout = new QFormLayout(this);
        tipo_.addItems({"Tarea", "Examen", "Proyecto"});
        fecha_.setCalendarPopup(true);
        fecha_.setDisplayFormat("yyyy-MM-dd");
        fecha_.setDate(QDate::currentDate());

        layout->addRow("Tipo", &tipo_);
        layout->addRow("Título", &titulo_);
        layout->addRow("Fecha", &fecha_);
        layout->addRow(&guardar_);
        layout->addRow(&importar_);
        layout->addRow(&lista_);
        layout->addRow(&estado_);
        connect(&importar_, &QPushButton::clicked, this, [this] {
            const auto ruta = QFileDialog::getOpenFileName(this, "Importar actividades", {}, "Archivos CSV (*.csv)");
            if (!ruta.isEmpty())
                emit cargaSolicitada(ruta);
        });
        connect(&guardar_, &QPushButton::clicked, this, [this] {
            emit registroSolicitado(tipo_.currentText(), titulo_.text(), fecha_.date());
        });
    }

    void mostrar(const Actividades& actividades) {
        lista_.clear();
        for (const auto& actividad : actividades)
            lista_.addItem(QString("%1 | %2 | %3")
                .arg(actividad->tipo(), actividad->titulo(), actividad->fecha().toString(Qt::ISODate)));
    }

    void indicar(const QString& mensaje) { estado_.setText(mensaje); }

signals:
    void cargaSolicitada(QString ruta);
    void registroSolicitado(QString tipo, QString titulo, QDate fecha);

private:
    QComboBox tipo_;
    QLineEdit titulo_;
    QDateEdit fecha_;
    QPushButton guardar_{"Registrar actividad"};
    QPushButton importar_{"Importar CSV"};
    QListWidget lista_;
    QLabel estado_;
};
