#include "cframe.h"
#include "ui_cframe.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>
#include <Producto.h>
#include <QInputDialog>
#include <QFileDialog>
#include <Pelicula.h>

cframe::cframe(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::cframe)
{
    ui->setupUi(this);
}

cframe::~cframe()
{
    delete ui;
}


void cframe::on_btnSalirMenu_clicked()
{

    ui->stackedWidget->setCurrentIndex(0);
}


void cframe::on_btnEjercicio1_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void cframe::on_btnEjercicio2_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void cframe::on_btnEjercicio3_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void cframe::on_btnReporte_clicked()
{
    QString inputFilePath = "C:\\Users\\harle\\OneDrive\\Documentos\\GitHub\\Laboratorio6_Harlen_Lopez_22311161\\estudiantes.txt";
    QString outputFilePath = "C:\\Users\\harle\\OneDrive\\Documentos\\GitHub\\Laboratorio6_Harlen_Lopez_22311161\\reporte.txt";

    QFile inputFile(inputFilePath);
    QFile outputFile(outputFilePath);

    qDebug() << "Intentando abrir el archivo de entrada en:" << inputFilePath;

    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "No se pudo abrir el archivo de entrada:" << inputFilePath;
        return;
    }

    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical() << "No se pudo abrir el archivo de salida:" << outputFilePath;
        return;
    }

    QTextStream in(&inputFile);
    QTextStream out(&outputFile);

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');

        if (fields.size() == 2) {
            QString nombre = fields.at(0).trimmed();
            int nota = fields.at(1).trimmed().toInt();

            QString resultado = (nota >= 60) ? "Aprobado" : "Reprobado";
            out << nombre << ": " << resultado << "\n";

        }
    }

    inputFile.close();
    outputFile.close();

    QMessageBox::information(this, "Reporte creado.", "Puede abrir el archivo en la ruta de este proyecto.");
}

QVector<Producto> cframe::leerProductosDesdeArchivo(const QString& nombreArchivo) {
    QFile archivo(nombreArchivo);
    QVector<Producto> listaProductos;

    if (!archivo.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "No se pudo abrir el archivo binario.");
        return listaProductos;
    }

    QDataStream in(&archivo);
    Producto producto;

    while (!in.atEnd()) {
        in >> producto;
        listaProductos.append(producto);
        qDebug() << "Producto leído:" << producto; // Mensaje de depuración
    }

    archivo.close();
    return listaProductos;
}

void cframe::guardarProductosEnArchivo(const QString& nombreArchivo)
{
    QFile archivo(nombreArchivo);

    if (!archivo.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Error", "No se pudo abrir el archivo para escritura.");
        return;
    }

    QDataStream out(&archivo);
    for (const Producto& producto : listaProductos) {
        out << producto;
    }

    archivo.close();
}

void cframe::on_btnActualizar_clicked()
{
    QString codigoProducto = ui->SPB_Codigo->text();

    if (codigoProducto.isEmpty()) {
        QMessageBox::warning(this, "Error", "Debe ingresar un código de producto válido.");
        return;
    }

    bool encontrado = false;
    for (Producto& producto : listaProductos) {
        if (producto.codigo == codigoProducto.toInt()) {
            encontrado = true;

            bool ok;
            int nuevaCantidad = QInputDialog::getInt(this, "Actualizar Stock", "Ingrese la nueva cantidad en stock:", producto.cantidadStock, 0, 10000, 1, &ok);

            if (ok) {
                producto.cantidadStock = nuevaCantidad;
                guardarProductosEnArchivo(codigoProducto);
                QMessageBox::information(this, "Éxito", "La cantidad de stock ha sido actualizada.");
            }
            break;
        }
    }

    if (!encontrado) {
        QMessageBox::warning(this, "Error", "No se encontró un producto con ese código.");
    }
}

void cframe::crearArchivoBinarioDePrueba()
{
    QVector<Producto> productosDePrueba;
    productosDePrueba.append(Producto(1, "Manzanas", 10.50, 100));
    productosDePrueba.append(Producto(2, "Sandias", 20.75, 150));
    productosDePrueba.append(Producto(3, "Fresas", 30.00, 300));
    productosDePrueba.append(Producto(4, "Melones", 40.00, 150));
    productosDePrueba.append(Producto(5, "Zapatos", 100.00, 80));
    productosDePrueba.append(Producto(6, "Pepsi", 25.00, 250));


    QString rutaFija = "C:\\Users\\harle\\OneDrive\\Documentos\\GitHub\\Laboratorio6_Harlen_Lopez_22311161\\archivo.bin";

    QFile file(rutaFija);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Error", "No se pudo abrir el archivo para escritura.");
        return;
    }

    QDataStream out(&file);
    for (const Producto& producto : productosDePrueba) {
        out << producto;
        qDebug() << "Producto guardado:" << producto;
        //QMessageBox::information(this,"Producto guardado:" << nuevaCantidad);
    }

    file.close();
    QMessageBox::information(this, "Éxito", "Archivo binario de prueba creado con éxito en " + rutaFija);
    qDebug() << "Archivo binario creado en:" << rutaFija;
}



void cframe::on_btnCargarProductos_2_clicked()
{
    crearArchivoBinarioDePrueba();
}


void cframe::on_btnCargarProductos_clicked()
{
    QString archivo = QFileDialog::getOpenFileName(this, "Seleccionar Archivo de Productos", "", "Archivos Binarios (*.bin)");

    if (archivo.isEmpty()) {
        QMessageBox::warning(this, "Error", "No se seleccionó ningún archivo.");
        return;
    }

    listaProductos = leerProductosDesdeArchivo(archivo);
}

QVector<Pelicula> leerPeliculasDesdeCSV(const QString& archivo)
{
    QVector<Pelicula> peliculas;
    QFile file(archivo);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(nullptr, "Error", "No se pudo abrir el archivo.");
        return peliculas;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString linea = in.readLine();
        QStringList campos = linea.split(',');

        if (campos.size() != 4) {
            continue;
        }

        QString titulo = campos[0];
        int year = campos[1].toInt();
        int duracion = campos[2].toInt();
        double calificacion = campos[3].toDouble();

        peliculas.append(Pelicula(titulo, year, duracion, calificacion));
    }

    file.close();
    return peliculas;
}

void calcularEstadisticas(const QVector<Pelicula>& peliculas, QString& resultado)
{
    if (peliculas.isEmpty()) {
        resultado = "No hay datos para calcular estadísticas.";
        return;
    }

    double sumaCalificaciones = 0.0;
    int cantidadPeliculas = peliculas.size();
    int cantidadAltas = 0;
    double umbral = 7.0;

    for (const Pelicula& pelicula : peliculas) {
        sumaCalificaciones += pelicula.calificacion;
        if (pelicula.calificacion > umbral) {
            cantidadAltas++;
        }
    }

    double promedioCalificacion = sumaCalificaciones / cantidadPeliculas;
    resultado = QString("Cantidad de películas: %1\nPromedio de calificación: %2\nCantidad de películas con calificación superior a %3: %4")
                    .arg(cantidadPeliculas)
                    .arg(promedioCalificacion)
                    .arg(umbral)
                    .arg(cantidadAltas);
}

void cframe::on_botonGenerarBIN_clicked()
{
    QVector<Pelicula> peliculas;
    peliculas.append(Pelicula("Inception", 2010, 148, 8.8));
    peliculas.append(Pelicula("The Dark Knight", 2008, 152, 9.0));
    peliculas.append(Pelicula("Interstellar", 2014, 169, 8.6));
    peliculas.append(Pelicula("The Matrix", 1999, 136, 8.7));
    peliculas.append(Pelicula("The Godfather", 1972, 175, 9.2));
    peliculas.append(Pelicula("Pulp Fiction", 1994, 154, 8.9));
    peliculas.append(Pelicula("The Shawshank Redemption", 1994, 142, 9.3));
    peliculas.append(Pelicula("Forrest Gump", 1994, 142, 8.8));
    peliculas.append(Pelicula("Fight Club", 1999, 139, 8.8));
    peliculas.append(Pelicula("The Lion King", 1994, 88, 8.5));
    peliculas.append(Pelicula("Gladiator", 2000, 155, 8.5));
    peliculas.append(Pelicula("The Departed", 2006, 151, 8.5));
    peliculas.append(Pelicula("The Prestige", 2006, 130, 8.5));
    peliculas.append(Pelicula("Braveheart", 1995, 178, 8.4));
    peliculas.append(Pelicula("Titanic", 1997, 195, 7.8));

    QString archivo = QFileDialog::getSaveFileName(this, "Guardar Archivo CSV", "", "Archivos CSV (*.csv)");

    if (archivo.isEmpty()) {
        QMessageBox::warning(this, "Error", "No se seleccionó ningún archivo.");
        return;
    }

    QFile file(archivo);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "No se pudo abrir el archivo para escritura.");
        return;
    }

    QTextStream out(&file);

    for (const Pelicula& pelicula : peliculas) {
        out << pelicula.titulo << ","
            << pelicula.year << ","
            << pelicula.duracion << ","
            << pelicula.calificacion << "\n";
    }

    file.close();
    QMessageBox::information(this, "Éxito", "Archivo CSV generado exitosamente.");
}


void cframe::on_botonCargarArchivo_clicked()
{
    QString nombreArchivo = QFileDialog::getOpenFileName(this, "Abrir archivo de películas", "", "Archivos CSV (*.csv);;Todos los archivos (*)");

    if (nombreArchivo.isEmpty()) {
        return;
    }

    QFile archivo(nombreArchivo);
    if (!archivo.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error", "No se pudo abrir el archivo.");
        return;
    }

    QTextStream in(&archivo);
    peliculas.clear();

    while (!in.atEnd()) {
        QString linea = in.readLine();
        QStringList campos = linea.split(',');

        if (campos.size() == 4) {
            Pelicula pelicula;
            pelicula.titulo = campos[0];
            pelicula.year = campos[1].toInt();
            pelicula.duracion = campos[2].toInt();
            pelicula.calificacion = campos[3].toDouble();

            peliculas.append(pelicula);
        }
    }

    archivo.close();
}

void cframe::guardarEstadisticasEnArchivo(const QString& archivo, double promedioCalificaciones, const QMap<int, int>& peliculasPorAno)
{
    qDebug() << "Entrando a guardarEstadisticasEnArchivo";
    QFile file(archivo);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "No se pudo abrir el archivo para escritura.");
        qDebug() << "Error al abrir el archivo para escritura:" << archivo;
        return;
    }

    QTextStream out(&file);

    out << "Promedio de calificaciones: " << promedioCalificaciones << "\n";
    out << "Cantidad de películas por año:\n";

    for (auto it = peliculasPorAno.begin(); it != peliculasPorAno.end(); ++it) {
        out << "Año " << it.key() << ": " << it.value() << " película(s)\n";
    }

    file.close();
    QMessageBox::information(this, "Éxito", "Archivo de estadísticas generado exitosamente.");
    qDebug() << "Archivo de estadísticas generado exitosamente en:" << archivo;
}

void cframe::on_botonEstadisticas_clicked()
{
    qDebug() << "Botón 'Generar Estadísticas' clickeado";

    if (peliculas.isEmpty()) {
        QMessageBox::warning(this, "No hay datos", "No se han cargado datos de películas.");
        qDebug() << "No se han cargado datos de películas.";
        return;
    }

    double sumaCalificaciones = 0.0;
    QMap<int, int> peliculasPorAno;
    int cantidadPeliculas = peliculas.size();

    for (const Pelicula& pelicula : peliculas) {
        sumaCalificaciones += pelicula.calificacion;

        if (peliculasPorAno.contains(pelicula.year)) {
            peliculasPorAno[pelicula.year]++;
        } else {
            peliculasPorAno[pelicula.year] = 1;
        }
    }

    double promedioCalificaciones = (cantidadPeliculas > 0) ? (sumaCalificaciones / cantidadPeliculas) : 0.0;

    QString archivo = QFileDialog::getSaveFileName(this, "Guardar Archivo de Estadísticas", "", "Archivos de Texto (*.txt)");

    if (archivo.isEmpty()) {
        QMessageBox::warning(this, "Error", "No se seleccionó ningún archivo.");
        qDebug() << "No se seleccionó ningún archivo.";
        return;
    }

    qDebug() << "Guardando estadísticas en el archivo:" << archivo;
    guardarEstadisticasEnArchivo(archivo, promedioCalificaciones, peliculasPorAno);
    qDebug() << "Estadísticas guardadas en el archivo:" << archivo;
}


void cframe::on_BtnSalir_clicked()
{
    QCoreApplication::quit();
}

