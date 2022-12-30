#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include "tree.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void console(std::string text);

private slots:
    void on_about_triggered(bool checked);

    void on_saveAs_triggered();

    void on_openFile_triggered();

    void on_searchInThree_pressed();

    void on_insertInThree_pressed();

    void on_deleteInThree_pressed();

    void on_new_file_triggered();

    void on_inorder_pressed();

    void on_preorder_pressed();

    void on_postorder_pressed();

    void on_checkBox_stateChanged(int arg1);

private:
    void setIsEdit(bool isEdit);
    void _tree_from_file_load(std::string filepath);
    void _reload_Qtree();
    void _treeViewer(node *_node, QTreeWidgetItem* result = NULL);

    const QString wTitle = "BTSearcher";
    bool is_setting_three_uncover = false;
    bool is_edit = false;
    Tree tree = Tree();
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
