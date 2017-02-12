#ifndef FMDB_H
#define FMDB_H

#include <QMainWindow>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlTableModel>

namespace Ui {
  class fmdb;
}

class fmdb : public QMainWindow
{
	Q_OBJECT

  public:
	explicit fmdb(QWidget *parent = 0);
	~fmdb();

  private slots:
	void on_action_Quit_triggered();
	void on_actionOpen_triggered();
	void on_actionSave_triggered();
	void on_actionRefresh_triggered();
	void on_actionAdd_triggered();
	void on_actionDel_triggered();

	void on_action_About_triggered();

  private:
	Ui::fmdb *ui;
	QString dbPath;
	QSqlDatabase db;
	QSqlTableModel *m;
	void resizeEvent(QResizeEvent*);
	void moveEvent(QMoveEvent*);
	void setupView();
	void setupDb(const QString);
	const QString fmdbVersion = "1.0.0";
};

#endif // FMDB_H
