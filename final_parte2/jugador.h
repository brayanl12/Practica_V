#ifndef JUGADOR_H
#define JUGADOR_H

#include <QString>
#include <QList>
#include "cannon.h"
#include "bloque.h"
#include <QGraphicsItem>

class Jugador
{
public:
    Jugador(const QString& nombre, Cannon* c);
    QString nombre;
    Cannon* cannon;
    QList<Bloque*> bloques;
    bool vivo;
    bool estaVivo() const;
    QGraphicsItem* personaje;
    void setPersonaje(QGraphicsItem* item);
    void recibirImpacto();
};

#endif // JUGADOR_H
