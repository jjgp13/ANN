#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <typeinfo>
#include <stdlib.h>

using namespace std;

string removeSpaces(string input){
  input.erase(remove(input.begin(),input.end(),' '),input.end());
  return input;
}

int calculateOutput(int theta, double weights[], double inputValues[][10], int nValues, int row){
    double sum = 0;
    //Sumatoria del input * el peso
    for(int i=0; i < nValues; i++) sum += inputValues[row][i] * weights[i];
    //Agregar el bias
    sum += weights[nValues];

    return (sum >= theta) ? 1 : 0;
}

//Funcion para extraer los valores de la lineas
vector<double> getValues(string str){
	vector<double> result;
    stringstream ss(str);
    while( ss.good() ){
        string substr;
        getline( ss, substr, ',' );
        std::string::size_type sz;
        result.push_back(stod(substr,&sz));
    }

	return result;
}

int main(int argc, char* argv[]) {
  string line;
  vector<string> lines;

  int d, m, n;
  static int MAX_ITER = 100;
  static double LEARNING_RATE = 0.1; 
  static int theta = 0;
  
  while (getline(cin,line)){
      //if(line == "0") break;
      lines.push_back(removeSpaces(line));
  }

  //Obtener los d, m y n 
  d = stoi(lines[0]);
  m = stoi(lines[1]);
  n = stoi(lines[2]);

  //Obtener training exmaples.
  double trainingExamplesInputs[m][10];
  double trainingExamplesOutputs[m];
  int y=0;
  for(int i = 3; i < 3 + m; i++){
      vector<double> vect = getValues(lines[i]);
      for(int j=0; j < d; j++) trainingExamplesInputs[y][j] = vect[j];
      trainingExamplesOutputs[y] = vect[vect.size()-1];
      y++;
  }

  //Arreglo con los pesos de cada input. d for inputs + 1 of the bias
  double weights[d+1];
  double localError, globalError;
  int i, p, iteration, output;

  //Inicializar los pesos en 0
  for(i = 0; i < d+1; i++) weights[i] = 0.5;

  iteration = 0;
  //Comenzar a calcular los errores en los pesos.
  do{
      iteration++;
      globalError = 0;
      for(i = 0; i < m; i++){
          //Calcular la salida del perceptron a partir de la funcion de activacion
          output = calculateOutput(theta, weights, trainingExamplesInputs, d, i);
          //Error local 
          localError = trainingExamplesOutputs[i] - output;

          //Actualizar los pesos y el bias;
          for(int w=0; w < d; w++) weights[w] += LEARNING_RATE * localError * trainingExamplesInputs[i][w];
          //bias
          weights[d] += LEARNING_RATE * localError;

          //Error global
          globalError += (localError*localError);
      }
  } while(globalError != 0 && iteration <= MAX_ITER);

  //Si despues de hacer un cierto numero de iteraciones no llega a un resultado 
  if(globalError != 0) {
      cout << "no solution found";
      return 0;
  }

  //Obtener test examples inputs.
  double tests[n][10];
  y=0;
  for(i = 3 + m; i < lines.size(); i++){
      vector<double> vect = getValues(lines[i]);
      for(int j=0; j < d; j++) tests[y][j] = vect[j];      
      y++;
  }

  //Calcular los outputs
  int row=0;
  for(i = 3 + m; i < lines.size(); i++){
      output = calculateOutput(theta, weights, tests, d, row);
      cout << output << endl;
      row++;
  }

  return 0;
}