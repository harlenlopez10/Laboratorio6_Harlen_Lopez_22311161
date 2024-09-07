#ifndef PRODUCTO_H
#define PRODUCTO_H

#include <QDataStream>
#include <QDebug>
#include <QString>

class Producto {
public:
    int codigo;
    QString nombre;
    double precio;
    int cantidadStock;

    Producto() : codigo(0), precio(0.0), cantidadStock(0) {}
    Producto(int codigo, const QString& nombre, double precio, int cantidadStock)
        : codigo(codigo), nombre(nombre), precio(precio), cantidadStock(cantidadStock) {}

    friend QDataStream& operator>>(QDataStream& in, Producto& producto);
    friend QDataStream& operator<<(QDataStream& out, const Producto& producto);
};

// Declaración del operador << para QDebug y Producto
QDebug operator<<(QDebug debug, const Producto &producto);

#endif // PRODUCTO_H
