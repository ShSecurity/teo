#include "ActividadController.h"
#include "MainWindow.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    RepositorioActividades repo;
    ActividadController controller(repo);
    MainWindow vista;

    QObject::connect(&vista, &MainWindow::registroSolicitado, &vista,
        [&](const QString& tipo, const QString& titulo, QDate fecha) {
            const bool registrada = controller.registrar(tipo, titulo, fecha);
            vista.indicar(registrada ? "Actividad registrada." : "Revise el título y la fecha.");
            vista.mostrar(controller.listar());
        });

    vista.show();
    if (app.arguments().contains("--smoke")) {
        emit vista.registroSolicitado("Tarea", "Práctica MVC", QDate(2026, 11, 21));
        app.processEvents();
        return controller.listar().size() == 1 ? 0 : 1;
    }
    return app.exec();
}
