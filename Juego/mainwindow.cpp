#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , dt(0.1) // Diferencial de tiempo
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 800, 600); // x, y, ancho, alto

    ui->graphicsView->setScene(scene);

    // Instanciar y agregar elementos a la escena
    // Usamos 'new' para crear los objetos dinámicamente
    Particula *p1 = new Particula(100, 100, 25, 30, 1.0, 10.0);
    scene->addItem(p1);
    particulas.push_back(p1);

    Particula *p2 = new Particula(600, 400, -35, -20, 1.5, 10.0);
    scene->addItem(p2);
    particulas.push_back(p2);

    Obstaculo *obs1 = new Obstaculo(350, 250, 100, 0.8);
    scene->addItem(obs1);
    obstaculos.push_back(obs1);

    // Configurar el motor de tiempo (bucle principal)
    timer = new QTimer(this);

    // Conectamos el timeout del timer a nuestra función actualizarFisica
    connect(timer, &QTimer::timeout, this, &MainWindow::actualizarFisica);

    // Arrancamos el temporizador para que se ejecute cada 16 milisegundos (~60 FPS)
    timer->start(16);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::actualizarFisica()
{
    // Actualizar posiciones matemáticas y gráficas
    for (Particula* p : particulas) {
        p->actualizarPosicion(dt);
    }

    // Verificar choques
    verificarColisiones();
}

void MainWindow::verificarColisiones()
{
    // Implementación de la lógica de los rebotes elásticos,
    // inelásticos y conservación de momento.
}
