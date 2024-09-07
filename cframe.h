#ifndef CFRAME_H
#define CFRAME_H

#include <QMainWindow>
#include <Producto.h>
#include <Pelicula.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class cframe;
}
QT_END_NAMESPACE

class cframe : public QMainWindow
{
    Q_OBJECT

public:
    cframe(QWidget *parent = nullptr);
    ~cframe();
    QVector<Producto> leerProductosDesdeArchivo(const QString& nombreArchivo);
    void guardarProductosEnArchivo(const QString& nombreArchivo, const QVector<Producto>& productos);
    void guardarProductosEnArchivo(const QString& nombreArchivo);
    void crearArchivoBinarioDePrueba();
    void guardarEstadisticasEnArchivo(const QString& archivo, double promedioCalificaciones, const QMap<int, int>& peliculasPorAno);

private slots:
;
    void on_btnSalirMenu_clicked();

void on_btnEjercicio1_clicked();

    void on_btnEjercicio2_clicked();

void on_btnEjercicio3_clicked();

    void on_btnReporte_clicked();

void on_btnActualizar_clicked();

    void on_btnCargarProductos_2_clicked();

void on_btnCargarProductos_clicked();

    void on_botonGenerarBIN_clicked();

void on_botonCargarArchivo_clicked();

    void on_botonEstadisticas_clicked();

void on_BtnSalir_clicked();

private:
    Ui::cframe *ui;
    QVector<Producto> listaProductos;
    QVector<Pelicula> peliculas;

};
#endif // CFRAME_H
