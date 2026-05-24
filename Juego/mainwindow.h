#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <vector>

#include "particula.h"
#include "obstaculo.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Este slot se ejecutará en cada latido del temporizador
    void actualizarFisica();

private:
    Ui::MainWindow *ui;

    QGraphicsScene *scene; // El mundo físico donde viven los objetos
    QTimer *timer;         // El motor de tiempo

    std::vector<Particula*> particulas;
    std::vector<Obstaculo*> obstaculos;

    double dt; // Diferencial de tiempo de la simulación

    // Método interno para limpiar el código de actualización
    void verificarColisiones();
};
#endif // MAINWINDOW_H
