#include "mainwindow.h"

#include "meshinitializer.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  qDebug() << "✓✓ MainWindow constructor";
  ui->setupUi(this);
  ui->applySubdivisionButton->setEnabled(false);
  ui->generalGroupBox->setEnabled(false);
}

MainWindow::~MainWindow() {
  qDebug() << "✗✗ MainWindow destructor";
  delete ui;
}

void MainWindow::importOBJ(QString filename) {
  qDebug() << filename;
  OBJFile newModel = OBJFile(filename);
  MeshInitializer initializer(&newModel);
  Mesh baseMesh = initializer.constructHalfEdgeMesh();

  meshes.clear();
  meshes.reserve(10);
  meshes.append(baseMesh);

  meshIndex = 0;
  updateBuffers();
  ui->MainDisplay->settings.modelLoaded = true;
  ui->generalGroupBox->setEnabled(true);
  ui->MainDisplay->update();
}

void MainWindow::on_ImportOBJ_clicked() {
  importOBJ(QFileDialog::getOpenFileName(this, "Import OBJ File", "models/",
                                         tr("Obj Files (*.obj)")));
}

void MainWindow::on_SubdivSteps_valueChanged(int value) {
  meshIndex = value;
  if (ui->MainDisplay->settings.requireApply) {
    return;
  }
  subdivide();
}

void MainWindow::on_wireframeCheckBox_toggled(bool checked) {
  ui->MainDisplay->settings.wireframeMode = checked;
  ui->MainDisplay->update();
}

void MainWindow::subdivide() {
  if (meshes.size() == 0) {
    return;
  }
  for (unsigned short k = meshes.size(); k < meshIndex + 1; k++) {
    meshes.append(Mesh());
    meshes[k - 1].subdivideCatmullClark(meshes[k]);
  }
  ui->MainDisplay->settings.uniformUpdateRequired = true;
  updateBuffers();
  ui->MainDisplay->update();
}

void MainWindow::updateBuffers() {
  ui->MainDisplay->updateBuffers(meshes[meshIndex]);
}

void MainWindow::on_applySubdivisionButton_pressed() { subdivide(); }

void MainWindow::on_requireApplyCheckBox_toggled(bool checked) {
  ui->MainDisplay->settings.requireApply = checked;
  ui->applySubdivisionButton->setEnabled(checked);
}

void MainWindow::on_importDefaultButton_pressed() {
  importOBJ(
      "/home/niels/Documents/parallel-catmull-clark-subdivision/src/models/"
      "QuadMeshes/Brick.obj");
}

void MainWindow::on_showNormalsCheckBox_toggled(bool checked) {
  ui->MainDisplay->settings.showNormals = checked;
  ui->MainDisplay->settings.uniformUpdateRequired = true;
  ui->MainDisplay->update();
}
