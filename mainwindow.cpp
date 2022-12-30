#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->horizontalLayoutWidget);
    this->setWindowTitle(this->wTitle);

    // Обработка открытия файла (передача пути файла в аргументы)
    if (QApplication::arguments().size() > 1)
    {
        const QString filepath = QApplication::arguments().at(1);
        this->_tree_from_file_load(filepath.toStdString());
    }
}

MainWindow::~MainWindow()
{
    this->tree.clear();
    delete ui;
}

void MainWindow::setIsEdit(bool isEdit)
{
    if (this->is_edit && !isEdit)
    {
        this->is_edit = false;
        this->setWindowTitle(this->windowTitle().replace("*", ""));
    } else if (!this->is_edit && isEdit)
    {
        this->is_edit = true;
        this->setWindowTitle("*" + this->windowTitle());
    }

}

void MainWindow::console(std::string text)
/*
  Метод выводит аргумент 'text' типа std::string
  в QTextEdit построчно
*/

{

   this->ui->consoleOutput->append(QString::fromStdString(text));
}

void MainWindow::_tree_from_file_load(std::string filepath)
{
    std::string filename = filepath.substr(filepath.find_last_of("/") + 1, std::string::npos);
    switch (this->tree.load(filepath))
    {
        case 0:
            this->ui->consoleOutput->clear();
            this->_reload_Qtree();
            this->console("Дерево успешно загружено");
            this->setWindowTitle(this->wTitle + " - " + QString::fromStdString(filename));
            this->setIsEdit(false);
            break;
        case 1:
            this->console("Ошибка открытия файла! '" + filename + "'");
            break;
        case 2:
            this->console("Файл пуст! '" + filename + "'");
            break;
        case 3:
            this->console("Файл поврежден! '" + filename + "'");
            break;
        default:
            this->console("Неизвестная ошибка!  '" + filename + "'");
    }
}

// Меню бар

void MainWindow::on_new_file_triggered()
{
    if (this->is_edit)
    {
        QMessageBox msgBox;
        msgBox.setText("Хотите сохранить текущее дерево?");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int res = msgBox.exec();
        if (res == QMessageBox::Ok)
            this->on_saveAs_triggered();

        if (res == QMessageBox::Cancel)
            return void();
    }
    this->setIsEdit(false);
    this->ui->consoleOutput->clear();
    this->tree.clear();
    this->_reload_Qtree();
    this->setWindowTitle(this->wTitle);
}

void MainWindow::on_about_triggered(bool checked)
/*
  Метод вызывает MessageBox c информацией о программе

  todo: версия не должна быть константой в коде
*/

{
    QMessageBox aboutBox;
    aboutBox.setWindowTitle("О программе");
    aboutBox.setText(
        "BTSearcher - программа, эмулирующая\n"
        "бинарное дерево поиска\n"
        "Версия: 1.1.3 \n\n"
        "Автор: Бояршинов Н. О.\n"
        "Группа: ВПР12\n\n"
        "г. Ростов-на-Дону | ДГТУ 2022 год"
    );
    aboutBox.setIcon(QMessageBox::Information);
    aboutBox.setStandardButtons(QMessageBox::Ok);
    aboutBox.exec();
}

void MainWindow::on_saveAs_triggered()
{
    QString filepath = QFileDialog::getSaveFileName(this, tr("Сохранить дерево"), "", tr("BTS Файл (*.bts)"));

    if (!filepath.isNull())
    {
        if (this->tree.save(filepath.toStdString()))
        {
            this->setIsEdit(false);
            this->console("Файл успешно сохранен");
        }
        else
            this->console("Ошибка сохранения файла!");
    }
}

void MainWindow::on_openFile_triggered()
/*
  Метод вызывает диалоговое окно
  загрузки файла дерева
*/

{
    QString filepath = QFileDialog::getOpenFileName(this, tr("Открыть файл дерева"), "", tr("BTS Файлы (*.bts);; TXT Файлы (*.txt)"));
    if (!filepath.isNull())
    {
        this->_tree_from_file_load(filepath.toStdString());
        this->setIsEdit(false);
    }
}


// Кнопки
void MainWindow::on_searchInThree_pressed()
{
    QInputDialog input_dialog;
    input_dialog.setWindowTitle("Введите число, которое хотите найти");
    input_dialog.setLabelText("Число:");
    input_dialog.setInputMode(QInputDialog::InputMode::IntInput);
    input_dialog.setIntMinimum(-2147483647);
    input_dialog.setIntMaximum(2147483647);

    int result = input_dialog.exec();
    if (result == QDialog::Accepted)
    {
        if (this->tree.search(input_dialog.intValue()))
            this->console("Значение '" +  std::to_string(input_dialog.intValue()) + "' успешно найдено!");
        else
            this->console("Значение '" +  std::to_string(input_dialog.intValue()) + "' не найдено!");
    }
}

void MainWindow::on_insertInThree_pressed()
{
    QInputDialog input_dialog;
    input_dialog.setWindowTitle("Введите число, которое хотите добавить");
    input_dialog.setLabelText("Число:");
    input_dialog.setInputMode(QInputDialog::InputMode::IntInput);
    input_dialog.setIntMinimum(-2147483647);
    input_dialog.setIntMaximum(2147483647);

    int result = input_dialog.exec();
    if (result == QDialog::Accepted)
    {
        if (this->tree.insert(input_dialog.intValue()))
        {
            this->console("Значение '" + std::to_string(input_dialog.intValue()) + "' успешно добавлено в дерево!");
            this->_reload_Qtree();
            this->setIsEdit(true);
        }
        else
            this->console("Значение '" +  std::to_string(input_dialog.intValue()) + "' не удалось добавить в дерево (дубликат)!");
    }
}

void MainWindow::on_deleteInThree_pressed()
{
    QInputDialog input_dialog;
    input_dialog.setWindowTitle("Введите число, которое хотите удалить");
    input_dialog.setLabelText("Число:");
    input_dialog.setInputMode(QInputDialog::InputMode::IntInput);
    input_dialog.setIntMinimum(-2147483647);
    input_dialog.setIntMaximum(2147483647);

    int result = input_dialog.exec();
    if (result == QDialog::Accepted)
    {
        if (this->tree.remove(input_dialog.intValue()))
        {
            this->console("Значение '" + std::to_string(input_dialog.intValue()) + "' успешно удалено!");
            this->_reload_Qtree();
            this->setIsEdit(true);
        }
        else
            this->console("Значение '" + std::to_string(input_dialog.intValue()) + "' не удалось удалить (не найдено)!");
    }
}

// Сортировки
void MainWindow::on_inorder_pressed()
{
    std::string result;
    if (this->tree.getRoot() != NULL)
    {
        result = "Inorder сортировка:\n";
        std::vector<int> values = this->tree.getInorderValues();
        for (int i = 0; i < values.size(); i++)
        {
            result += std::to_string(values[i]) + " ";
        }
    } else
        result = "[Ошибка] Дерево не содержит элементов!";
    this->console(result);
}

void MainWindow::on_preorder_pressed()
{
    std::string result;
    if (this->tree.getRoot() != NULL)
    {
        result = "Preorder сортировка:\n";
        std::vector<int> values = this->tree.getPreorderValues();
        for (int i = 0; i < values.size(); i++)
        {
            result += std::to_string(values[i]) + " ";
        }
    } else
        result = "[Ошибка] Дерево не содержит элементов!";
    this->console(result);
}

void MainWindow::on_postorder_pressed()
{
    std::string result;
    if (this->tree.getRoot() != NULL)
    {
        result = "Postorder сортировка:\n";
        std::vector<int> values = this->tree.getPostorderValues();
        for (int i = 0; i < values.size(); i++)
        {
            result += std::to_string(values[i]) + " ";
        }
    } else
        result = "[Ошибка] Дерево не содержит элементов!";
    this->console(result);
}


// utils
void MainWindow::_reload_Qtree()
{
    ui->treeWidget->clear();
    if (this->tree.getRoot() != NULL )
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
        item->setText(0, "Дерево");
        this->_treeViewer(this->tree.getRoot(), item);
        if (is_setting_three_uncover)
            this->ui->treeWidget->expandAll();
    }
}

void MainWindow::_treeViewer(node *_node, QTreeWidgetItem* tree)
/*
 * Отрисовка виджета QTreeWidget
 */
{
    if (_node != nullptr)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(tree);
        item->setText(0, QString::fromStdString(std::to_string(_node->key)));

        // Обходим лево
        QTreeWidgetItem *item_left = new QTreeWidgetItem(item);
        item_left->setText(0, "Лево");
        this->_treeViewer(_node->left, item_left);
        if (item_left->childCount() == 0)
            item->removeChild(item_left);

        // Обходим право
        QTreeWidgetItem *item_right = new QTreeWidgetItem(item);
        item_right->setText(0, "Право");
        this->_treeViewer(_node->right, item_right);
        if (item_right->childCount() == 0)
            item->removeChild(item_right);
    }
}



void MainWindow::on_checkBox_stateChanged(int state)
{
    if (state)
        this->is_setting_three_uncover = true;
    else
        this->is_setting_three_uncover = false;
    this->_reload_Qtree();
}

