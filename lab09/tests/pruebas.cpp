#include "ActividadController.h"
#include "CsvRepositorioAdapter.h"
#include <QTemporaryFile>
#include <QtTest>

class Pruebas : public QObject {
    Q_OBJECT

private slots:
    void crearTresTipos() {
        const QDate fecha(2026, 11, 21);
        auto tarea = crearActividad(" TAREA ", " Informe ", fecha);
        auto examen = crearActividad("examen", "Parcial", fecha);
        auto proyecto = crearActividad("proyecto", "Entrega", fecha);
        QVERIFY(dynamic_cast<Tarea*>(tarea.get()));
        QVERIFY(dynamic_cast<Examen*>(examen.get()));
        QVERIFY(dynamic_cast<Proyecto*>(proyecto.get()));
        QCOMPARE(tarea->titulo(), QString("Informe"));
        QCOMPARE(tarea->fecha(), fecha);
    }

    void rechazarCreacionInvalida() {
        const QDate fecha(2026, 11, 21);
        QVERIFY(!crearActividad("otro", "Informe", fecha));
        QVERIFY(!crearActividad("tarea", "  ", fecha));
        QVERIFY(!crearActividad("tarea", "Informe", {}));
    }

    void registrarYListar() {
        RepositorioActividades repo;
        CsvRepositorioAdapter csv;
        ActividadController controlador(repo, csv);
        QVERIFY(!controlador.registrar("examen", "", QDate(2026, 11, 21)));
        QVERIFY(controlador.listar().empty());
        QVERIFY(controlador.registrar("tarea", "Informe", QDate(2026, 11, 21)));
        QCOMPARE(controlador.listar().size(), std::size_t(1));
        QCOMPARE(controlador.listar().front()->titulo(), QString("Informe"));
    }

    void importarCsvValido() {
        QTemporaryFile archivo;
        QVERIFY(archivo.open());
        QVERIFY(archivo.write("tipo;titulo;fecha\nexamen;Parcial;2026-11-25\nproyecto;Entrega;2026-11-30\n") > 0);
        archivo.close();
        RepositorioActividades repo;
        CsvRepositorioAdapter csv;
        ActividadController controlador(repo, csv);
        QCOMPARE(controlador.cargar(archivo.fileName()), std::size_t(2));
        QCOMPARE(controlador.listar().back()->tipo(), QString("Proyecto"));
    }

    void importacionInvalidaConservaEstado_data() {
        QTest::addColumn<QByteArray>("fila");
        QTest::newRow("fecha") << QByteArray("examen;Parcial;2026-02-30");
        QTest::newRow("tipo") << QByteArray("otro;Parcial;2026-11-25");
        QTest::newRow("campos") << QByteArray("examen;Parcial");
        QTest::newRow("titulo") << QByteArray("examen; ;2026-11-25");
    }

    void importacionInvalidaConservaEstado() {
        QFETCH(QByteArray, fila);
        QTemporaryFile archivo;
        QVERIFY(archivo.open());
        QVERIFY(archivo.write("tarea;Primera valida;2026-11-23\n" + fila + "\n") > 0);
        archivo.close();
        RepositorioActividades repo;
        CsvRepositorioAdapter csv;
        ActividadController controlador(repo, csv);
        QVERIFY(controlador.registrar("tarea", "Existente", QDate(2026, 11, 21)));
        QVERIFY_EXCEPTION_THROWN(controlador.cargar(archivo.fileName()), std::runtime_error);
        QCOMPARE(controlador.listar().size(), std::size_t(1));
        QCOMPARE(controlador.listar().front()->titulo(), QString("Existente"));
    }

    void archivoInexistente() {
        QTemporaryFile archivo;
        QVERIFY(archivo.open());
        const auto ruta = archivo.fileName();
        QVERIFY(archivo.remove());
        CsvRepositorioAdapter csv;
        QVERIFY_EXCEPTION_THROWN(csv.cargarTodo(ruta), std::runtime_error);
    }
};

QTEST_APPLESS_MAIN(Pruebas)
#include "pruebas.moc"
