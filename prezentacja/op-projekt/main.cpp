/* 
 * Praktyka optymalizacji - Simulaed Anneling for the Set-partitioning Problem
*/

#include <stdio.h>
#include <stdlib.h>
#define SIZE_OF_ROUTES 3
#include <time.h>
#define ALPHA 0.95

/* Function print matrix */
void printMatrix(int** matrix, int row, int col)
{
	int i, j;

	for (i = 0; i < row; i++)
	{
		for(j = 0; j < col; j++)
			printf("%d ", matrix[i][j]);
		printf("\n");
	}
}

/* Function cost count total cost of solution */
float cost(int** solution, float** distance, int numberOfRoutes)
{
  float cost = 0;
  
  for (int i = 0; i < numberOfRoutes; i++)
    for (int j = 0; j < SIZE_OF_ROUTES; j++)
      if (solution[i][j] == 0)
      {
	if (solution[i][j-1] != 0)
	  cost += distance[0][solution[i][j-1]];	  
      }
      else
      {
	if (j == 0)
	  cost += distance[0][solution[i][j]];
	else if (j == SIZE_OF_ROUTES - 1)
	  cost += distance[solution[i][j-1]][solution[i][j]] + distance[0][solution[i][j]];
	else
	  cost += distance[solution[i][j-1]][solution[i][j]];
      }      
      
  return cost;    
}

/* Annealing Step */
int annealing_step(int** old_solution, int** best_solution, float** distance, int numberOfRoutes, int numberOfCustomers, int temperature)
{
  int i, j;
  int randomCustomer = rand() % numberOfCustomers + 1;
  
  /* Seeks to route which a client should be drawn */
  int randomCustomerRoute;
  bool found = false;
  for (i = 0; i < numberOfRoutes; i++)
  {
    for(j = 0; j < SIZE_OF_ROUTES; j++)
      if (best_solution[i][j] == randomCustomer)
      {
	randomCustomerRoute = i;
	found = true;
	break;
      }
      
      if (found)
	break;
  }
  
  /* draws random route */
  int randomRoute;
  int randomRouteSize = 0;
  do
  {
     randomRoute = rand() % numberOfRoutes;
  }
  while (randomRoute == randomCustomerRoute);
    
  for (i = 0; i < SIZE_OF_ROUTES; i++)
    if (old_solution[randomRoute][i] != 0)
      randomRouteSize++;
  
  /* create new_solution */
  int** new_solution;
	
  i = numberOfRoutes;
  new_solution = new int*[i];
  while(i--)
  new_solution[i] = new int[3];
  
  for (i = 0; i < numberOfRoutes; i++)
    for(j = 0; j< SIZE_OF_ROUTES; j++)
      new_solution[i][j] = old_solution[i][j];
  
  /* Create the new solution by moving the customer into the chosen route */ 
  if (randomRouteSize < SIZE_OF_ROUTES)
  {
    for (i = 0; i < SIZE_OF_ROUTES; i++)
      if (new_solution[randomRoute][i] == 0)
      {
	new_solution[randomRoute][i] = randomCustomer;
	break;	
      }
      
    for (i = 0; i < SIZE_OF_ROUTES; i++)
      if (new_solution[randomCustomerRoute][i] == randomCustomer)
      {
	new_solution[randomCustomerRoute][i] = 0;
	break;
      }
      
    for (i = 0; i < SIZE_OF_ROUTES - 1; i++)
      if (new_solution[randomCustomerRoute][i] == 0 && new_solution[randomCustomerRoute][i+1] != 0)
      {
	new_solution[randomCustomerRoute][i] = new_solution[randomCustomerRoute][i+1];
	new_solution[randomCustomerRoute][i+1] = 0;
      }
  }
  /* Selec randomly a customer in the route and exchanging selected customers betweend their routes */
  else
  {
    int randomRouteElement;
    do   
      randomRouteElement = rand() % SIZE_OF_ROUTES;
    while (old_solution[randomRoute][randomRouteElement] == 0);
    
    for (i = 0; i < SIZE_OF_ROUTES; i++)
      if (new_solution[randomCustomerRoute][i] == randomCustomer)
      {
	new_solution[randomCustomerRoute][i] = old_solution[randomRoute][randomRouteElement];
	break;
      }
      
    new_solution[randomRoute][randomRouteElement] = randomCustomer;
  }
  
  /* Compute cost of new solution */
  float costNewSolution = cost(new_solution, distance, numberOfRoutes);
  float delta = costNewSolution - cost(old_solution, distance, numberOfRoutes);
  float x;
  
  x = 1.0 / (rand() % 10 + 1.1);
  
  if (delta < 0 || x < temperature / (temperature + delta))
  {
    for (i = 0; i < numberOfRoutes; i ++)
      for (j = 0; j < SIZE_OF_ROUTES; j++)
	old_solution[i][j] = new_solution[i][j];
      
    if (costNewSolution < cost(best_solution, distance, numberOfRoutes))
    {
      for (i = 0; i < numberOfRoutes; i ++)
	for (j = 0; j < SIZE_OF_ROUTES; j++)
	  best_solution[i][j] = new_solution[i][j];
	     
      return 0;
    }
  }
  
  return 1;
}


int main(int argc, char *argv[])
{
  
  int numberOfCustomers, i, j;
  int equilibrium_counter = 0;
  float temperature;
  srand (time(NULL));

  scanf("%d", &numberOfCustomers);
  i = numberOfCustomers + 1;

  /* Creat matrix and read distance between customers */
  float** distance; 
  distance = new float*[i]; 
	
  while(i--) 
    distance[i] = new float[numberOfCustomers + 1];
		
  for (i = 0; i <= numberOfCustomers; i++)
    for (j = 0; j <= numberOfCustomers; j++)
      scanf("%f", &distance[i][j]);
	
  /* Create old solution as the set of random routes */
  int numberOfRoutes;
  if (numberOfCustomers % SIZE_OF_ROUTES == 0) 
    numberOfRoutes = numberOfCustomers / SIZE_OF_ROUTES;
  else
    numberOfRoutes = numberOfCustomers / SIZE_OF_ROUTES + 1;
	
  int** old_solution;
  bool drown[numberOfCustomers];
  int m = 0;
	
  i = numberOfRoutes;
  old_solution = new int*[i];
  while(i--)
    old_solution[i] = new int[3];
	  
  for (i = 0; i < numberOfCustomers; i++)
    drown[i] = false;
	
  for (i = 0; i < numberOfRoutes; i++)
    for (j = 0; j < SIZE_OF_ROUTES; j++)
  
  if (m < numberOfCustomers)
  {
    int d = rand() % numberOfCustomers;
    bool added = false;
	      
    while (!added)
      if (drown[d] == false)
      {
	drown[d] = true;
	old_solution[i][j] = d+1;
	added = true;
	m++;
      }
      else
	d = (d+1) % numberOfCustomers;	      
  }
  	  
  /* Create best solution */
  int** best_solution;
	
  i = numberOfRoutes;
  best_solution = new int*[i];
  while(i--)
  best_solution[i] = new int[3];

  /* best solution = old solution */
  for(i = 0; i < numberOfRoutes; i++)
    for(j = 0; j < SIZE_OF_ROUTES; j++)
      best_solution[i][j] = old_solution[i][j];

  temperature = cost(best_solution, distance, numberOfRoutes) / 1000;  
 
  /* main loop */
  do
  {
    for (int iteration_counter = 1; i <= numberOfCustomers * numberOfCustomers; i++)
      equilibrium_counter *= annealing_step(old_solution, best_solution, distance, numberOfRoutes, numberOfCustomers, temperature);
	  
    temperature *= ALPHA;
    equilibrium_counter++;
  }
  while (equilibrium_counter <= 20);
  
  printMatrix(best_solution, numberOfRoutes, SIZE_OF_ROUTES);
  printf("\n Total cost: %f\n", cost(best_solution, distance, numberOfRoutes));
		  
  return 0;
}