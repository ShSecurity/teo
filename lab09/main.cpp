#include "ActividadController.h"
#include "MainWindow.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    RepositorioActividades repo;
    CsvRepositorioAdapter csv;
    ActividadController controller(repo, csv);
    MainWindow vista;

    QObject::connect(&vista, &MainWindow::registroSolicitado, &vista,
        [&](const QString& tipo, const QString& titulo, QDate fecha) {
            const bool registrada = controller.registrar(tipo, titulo, fecha);
            vista.indicar(registrada ? "Actividad registrada." : "Revise el título y la fecha.");
            vista.mostrar(controller.listar());
        });

    QObject::connect(&vista, &MainWindow::cargaSolicitada, &vista, [&](const QString& ruta) {
        try {
            const auto cantidad = controller.cargar(ruta);
            vista.mostrar(controller.listar());
            vista.indicar(QString("Actividades importadas: %1").arg(cantidad));
        } catch (const std::exception& error) {
            vista.indicar(QString::fromUtf8(error.what()));
        }
    });

    vista.show();
    if (app.arguments().contains("--smoke")) {
        emit vista.registroSolicitado("Tarea", "Práctica MVC", QDate(2026, 11, 21));
        app.processEvents();
        return controller.listar().size() == 1 ? 0 : 1;
    }
    return app.exec();
}
