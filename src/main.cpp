#include <QApplication>
#include <QElapsedTimer>
#include <QSurfaceFormat>
#include <iostream>

#include "mainwindow.h"
#include "meshinitializer.h"
#include "objfile.h"
#include "omp.h"
#include "quadmesh.h"

/**
 * @brief getCmdOption Retrieves the string after the provided flag
 * @param begin Start of the array to be searched in
 * @param end End of the array to be searched in
 * @param option Flag
 * @return The string after the provided flag if it exists, null otherwise
 */
char* getCmdOption(char** begin, char** end, const std::string& option) {
  char** itr = std::find(begin, end, option);
  if (itr != end && ++itr != end) {
    return *itr;
  }
  return nullptr;
}

/**
 * @brief cmdOptionExists Finds whether a command flag exists
 * @param begin Start of the array to be searched in
 * @param end End of the array to be searched in
 * @param option The flag
 * @return True if the flag is found, false otherwise
 */
bool cmdOptionExists(char** begin, char** end, const std::string& option) {
  return std::find(begin, end, option) != end;
}

void subdivide(Mesh* baseMesh, int subdivisionLevel) {
  Mesh* currentMesh;
  QElapsedTimer timer;
  timer.start();
  currentMesh = baseMesh;
  for (unsigned short k = 0; k < subdivisionLevel; k++) {
    QuadMesh* newMesh = new QuadMesh();
    QElapsedTimer subTimer;
    subTimer.start();
    currentMesh->subdivideCatmullClark(*newMesh);
    /* Display info to user */
    long long subTime = subTimer.nsecsElapsed();
    std::cout << "Subdivision time at " << k + 1 << " is "
              << subTime / 1000000.0 << " milliseconds\n";
    currentMesh = newMesh;
  }
  /* Display info to user */
  long long time = timer.nsecsElapsed();
  double milsecs = time / 1000000.0;
  std::cout << "\n---\nTotal time elapsed for " << subdivisionLevel << " : "
            << milsecs << " milliseconds\n\n";
  std::cout << "Faces : " << currentMesh->getNumFaces() << "\n";
  std::cout << "Half Edges : " << currentMesh->getNumHalfEdges() << "\n";
  std::cout << "Vertices : " << currentMesh->getNumVerts() << "\n";
  std::cout << "Edges : " << currentMesh->getNumEdges() << "\n\n";
}

int startGUI(int argc, char* argv[]) {
  QApplication a(argc, argv);
  QSurfaceFormat glFormat;
  glFormat.setProfile(QSurfaceFormat::CoreProfile);
  glFormat.setVersion(4, 1);
  glFormat.setOption(QSurfaceFormat::DebugContext);
  QSurfaceFormat::setDefaultFormat(glFormat);

  MainWindow w;
  w.show();
  return a.exec();
}

int main(int argc, char* argv[]) {
  //  omp_set_dynamic(false);
  omp_set_num_threads(4);
  bool usingGUI = true;

  if (cmdOptionExists(argv, argv + argc, "-c")) {
    usingGUI = false;
  }

  if (usingGUI) {
    return startGUI(argc, argv);
  }

  char* filename = getCmdOption(argv, argv + argc, "-f");
  char* subdivLevel = getCmdOption(argv, argv + argc, "-l");
  char* numThreads = getCmdOption(argv, argv + argc, "-t");

  if (filename == nullptr || subdivLevel == nullptr || numThreads == nullptr) {
    std::string executable = argv[0];
    std::cerr << "Usage: "
              << executable.substr(executable.find_last_of('/') + 1)
              << " -g\n or \n"
              << executable.substr(executable.find_last_of('/') + 1)
              << " -t <num threads> -f <filename> -l <subdivision level>\n";
    exit(EXIT_FAILURE);
  }
  omp_set_num_threads(atoi(numThreads));
  OBJFile newModel = OBJFile(QString(filename));
  MeshInitializer initializer(&newModel);
  Mesh* baseMesh = initializer.constructHalfEdgeMesh();
  subdivide(baseMesh, atoi(subdivLevel));

  exit(EXIT_SUCCESS);
}
