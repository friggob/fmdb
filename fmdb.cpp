#include "fmdb.h"
#include "ui_fmdb.h"
#include <QFileDialog>
#include <QDebug>
#include <QSqlRecord>
#include <QDateTime>
#include <QMessageBox>


fmdb::~fmdb()
{
  delete ui;
}

fmdb::fmdb(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::fmdb)
{
  dbPath = "/ds/filmer/fmdb.db2";

  setupDb(dbPath);

  ui->setupUi(this);
  setupView();
  ui->statusBar->showMessage(QString::asprintf("x=%d, y=%d, Width=%d, Height=%d",this->x(),this->y(),this->width(),this->height()));
}

void fmdb::setupDb(const QString path){
  db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(path);

  if(!db.open()){
	qDebug() << "Error: database connection failed!";
  }else{
	qDebug() << "Database connection is ok!";
  }
}

void fmdb::setupView(){
  m = new QSqlTableModel(this);
  QHeaderView *vh = ui->fmdbView->verticalHeader();

  m->setEditStrategy(QSqlTableModel::OnManualSubmit);
  m->setTable("fmdb");
  m->select();
  m->setHeaderData(0,Qt::Horizontal,tr("Title"));
  m->setHeaderData(3,Qt::Horizontal,tr("Rating"));
  m->setHeaderData(1,Qt::Horizontal,tr("IMDB Link"));
  m->setHeaderData(2,Qt::Horizontal,tr("Date watched"));

  ui->fmdbView->setModel(m);
  ui->fmdbView->resizeColumnsToContents();
  ui->fmdbView->horizontalHeader()->moveSection(3,0);
  ui->fmdbView->sortByColumn(2,Qt::DescendingOrder);

  vh->setSectionResizeMode(QHeaderView::Fixed);
  vh->setDefaultSectionSize(18);
}

void fmdb::moveEvent(QMoveEvent *event){
  QMainWindow::moveEvent(event);
  ui->statusBar->showMessage(QString::asprintf("x=%d, y=%d, Width=%d, Height=%d",this->x(),this->y(),this->width(),this->height()));
}

void fmdb::resizeEvent(QResizeEvent* event){
  QMainWindow::resizeEvent(event);
  ui->statusBar->showMessage(QString::asprintf("x=%d, y=%d, Width=%d, Height=%d",this->x(),this->y(),this->width(),this->height()));
}

void fmdb::on_action_Quit_triggered()
{
  close();
}

void fmdb::on_actionOpen_triggered()
{
  dbPath = QFileDialog::getOpenFileName(this,tr("Open database"),"/ds/filmer",tr("Database files (*db);;Any files (*)"));
  qDebug() << dbPath;
  m->clear();
  setupDb(dbPath);
  setupView();
}

void fmdb::on_actionSave_triggered()
{
  if(m->submitAll()){
	qDebug() << "Updated ok!";
	m->select();
  }else{
	qDebug() << "Updating failed!";
  }
}

void fmdb::on_actionRefresh_triggered()
{
	m->clear();
	setupView();
}

void fmdb::on_actionAdd_triggered()
{
  QSqlRecord rec = m->record();
  rec.setValue(2,QVariant(QDate::currentDate()));
  m->insertRecord(0,rec);
}

void fmdb::on_actionDel_triggered()
{
  QModelIndexList rows;
  rows = ui->fmdbView->selectionModel()->selectedRows();
  if(rows.isEmpty()){
	qDebug() << "No rows selected!";
  }else{
	qDebug() << rows.count() << "seleted!";

	m->removeRows(rows.first().row(),rows.count());
  }
}

void fmdb::on_action_About_triggered()
{
  QString msg;
  msg = "fmdb version v" + fmdbVersion;
  msg += "\n\nCreated by Fredrik Olausson <gh@jfo.im>\n";
  QMessageBox::about(this,"About fmdb",msg);
}
