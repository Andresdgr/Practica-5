#include "mainwindow.h"
#include <cmath>     // Para std::sqrt y std::abs
#include <algorithm> // Para std::find y std::remove
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
    Particula *p1 = new Particula(100, 100, 0, 5, 1.0, 10.0);
    scene->addItem(p1);
    particulas.push_back(p1);

    Particula *p2 = new Particula(100, 400, 0, -5, 1.5, 10.0);
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
    // Colisiones con los límites de la ventana (Elásticas)
    for (Particula* p : particulas) {
        Vector2D pos = p->getPosicion();
        Vector2D vel = p->getVelocidad();
        double r = p->getRadio();

        // Paredes laterales (Eje X)
        // Se resta o suma el radio para que rebote el borde, no el centro
        if (pos.getX() - r <= 0 || pos.getX() + r >= scene->width()) {
            p->setVelocidad(-vel.getX(), vel.getY());
        }

        // Techo y suelo (Eje Y)
        if (pos.getY() - r <= 0 || pos.getY() + r >= scene->height()) {
            p->setVelocidad(vel.getX(), -vel.getY());
        }
    }

    // Colisiones con los obstáculos
    for (Particula* p : particulas) {
        for (Obstaculo* obs : obstaculos) {
            Vector2D pos = p->getPosicion();
            double r = p->getRadio();

            // Coordenadas del obstáculo
            double obsX = obs->getPosicion().getX();
            double obsY = obs->getPosicion().getY();
            double obsLado = obs->getLado();

            // Encontrar el punto del obstáculo más cercano al centro de la partícula
            double testX = pos.getX();
            double testY = pos.getY();

            if (pos.getX() < obsX) testX = obsX;                           // Borde izquierdo
            else if (pos.getX() > obsX + obsLado) testX = obsX + obsLado;  // Borde derecho

            if (pos.getY() < obsY) testY = obsY;                           // Borde superior
            else if (pos.getY() > obsY + obsLado) testY = obsY + obsLado;  // Borde inferior

            // Calcular distancia mediante Pitágoras
            double distX = pos.getX() - testX;
            double distY = pos.getY() - testY;
            double distancia = std::sqrt((distX * distX) + (distY * distY));

            if (distancia > 0 && distancia <= r) {
                Vector2D vel = p->getVelocidad();

                // Vector normalizado (nx, ny) que apunta desde el obstáculo hacia la partícula
                double nx = distX / distancia;
                double ny = distY / distancia;

                // Calculamos el producto punto
                double productoPunto = (vel.getX() * nx) + (vel.getY() * ny);

                // Se aplica el choque si se están acercando
                if (productoPunto < 0) {
                    double e = obs->getE();

                    if (std::abs(distX) > std::abs(distY)) {
                        p->setVelocidad(-vel.getX() * e, vel.getY()); // Rebote horizontal
                    } else {
                        p->setVelocidad(vel.getX(), -vel.getY() * e); // Rebote vertical
                    }
                }
            }
        }
    }

    // Colisiones entre partículas
    std::vector<Particula*> particulasAEliminar;
    std::vector<Particula*> particulasAAgregar;

    for (size_t i = 0; i < particulas.size(); i++) {
        for (size_t j = i + 1; j < particulas.size(); j++) {
            Particula* p1 = particulas[i];
            Particula* p2 = particulas[j];

            // Si alguna partícula ya chocó en este frame y está en la lista de eliminación, la ignoramos
            if (std::find(particulasAEliminar.begin(), particulasAEliminar.end(), p1) != particulasAEliminar.end() ||
                std::find(particulasAEliminar.begin(), particulasAEliminar.end(), p2) != particulasAEliminar.end()) {
                continue;
            }

            double dx = p2->getPosicion().getX() - p1->getPosicion().getX();
            double dy = p2->getPosicion().getY() - p1->getPosicion().getY();
            double distancia = std::sqrt(dx * dx + dy * dy);

            if (distancia <= (p1->getRadio() + p2->getRadio())) {
                // Conservación del momento lineal
                double m1 = p1->getMasa();
                double m2 = p2->getMasa();
                double masaTotal = m1 + m2;

                double vxf = (m1 * p1->getVelocidad().getX() + m2 * p2->getVelocidad().getX()) / masaTotal;
                double vyf = (m1 * p1->getVelocidad().getY() + m2 * p2->getVelocidad().getY()) / masaTotal;

                // Propiedades del nuevo cuerpo
                double nuevoX = (p1->getPosicion().getX() + p2->getPosicion().getX()) / 2.0; // Aparece en el medio
                double nuevoY = (p1->getPosicion().getY() + p2->getPosicion().getY()) / 2.0;
                // Para conservar una proporción visual lógica, sumamos las áreas y sacamos el nuevo radio
                double nuevoRadio = p1->getRadio() + p1->getRadio() / 3;

                // Creamos el nuevo objeto dinámico
                Particula* pNueva = new Particula(nuevoX, nuevoY, vxf, vyf, masaTotal, nuevoRadio);

                // Encolamos las acciones
                particulasAAgregar.push_back(pNueva);
                particulasAEliminar.push_back(p1);
                particulasAEliminar.push_back(p2);
            }
        }
    }

    // Actualizamos la memoria y la interfaz gráfica de forma segura
    for (Particula* pDel : particulasAEliminar) {
        scene->removeItem(pDel); // Quita el dibujo
        // Idioma Erase-Remove: Elimina el puntero del vector principal
        particulas.erase(std::remove(particulas.begin(), particulas.end(), pDel), particulas.end());
        delete pDel; // Libera la memoria en el Heap
    }
    for (Particula* pAdd : particulasAAgregar) {
        scene->addItem(pAdd);
        particulas.push_back(pAdd);
    }
}
