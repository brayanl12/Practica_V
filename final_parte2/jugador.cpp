#include "jugador.h"

Jugador::Jugador(const QString& n, Cannon* c)
{
    nombre = n;
    cannon = c;
    vivo = true;
    personaje = nullptr;
}

bool Jugador::estaVivo() const
{
    return vivo;
}

void Jugador::setPersonaje(QGraphicsItem* item)
{
    personaje = item;
}

void Jugador::recibirImpacto()
{
    vivo = false;

    //efecto visual al recibir impacto
    if (personaje) {
        personaje->setOpacity(0.3);
    }
}
