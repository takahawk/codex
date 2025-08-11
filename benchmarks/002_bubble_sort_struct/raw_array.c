#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
  int id;
  char *name;
  char *surname;
  int age;
} Person;

char *first_names[] = {
    "James", "Mary", "John", "Patricia", "Robert", "Jennifer", "Michael", "Linda",
    "William", "Elizabeth", "David", "Barbara", "Richard", "Susan", "Joseph", "Jessica",
    "Thomas", "Sarah", "Christopher", "Karen", "Charles", "Nancy", "Daniel", "Lisa",
    "Matthew", "Betty", "Anthony", "Helen", "Mark", "Sandra", "Donald", "Donna",
    "Steven", "Carol", "Paul", "Ruth", "Andrew", "Sharon", "Joshua", "Michelle",
    "Kenneth", "Laura", "Kevin", "Sarah", "Brian", "Kimberly", "George", "Deborah",
    "Timothy", "Dorothy", "Ronald", "Lisa", "Jason", "Nancy", "Edward", "Karen",
    "Jeffrey", "Betty", "Ryan", "Helen", "Jacob", "Sandra", "Gary", "Donna",
    "Nicholas", "Carol", "Eric", "Ruth", "Jonathan", "Sharon", "Stephen", "Michelle",
    "Larry", "Laura", "Justin", "Sarah", "Scott", "Kimberly", "Brandon", "Deborah",
    "Benjamin", "Dorothy", "Samuel", "Amy", "Gregory", "Angela", "Alexander", "Ashley",
    "Patrick", "Brenda", "Frank", "Emma", "Raymond", "Olivia", "Jack", "Cynthia",
    "Dennis", "Marie", "Jerry", "Janet", "Tyler", "Catherine", "Aaron", "Frances",
    "Jose", "Christine", "Henry", "Samantha", "Adam", "Debra", "Douglas", "Rachel",
    "Nathan", "Carolyn", "Peter", "Virginia", "Zachary", "Maria", "Kyle", "Heather"
};

char *surnames[] = {
    "Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia", "Miller", "Davis",
    "Rodriguez", "Martinez", "Hernandez", "Lopez", "Gonzalez", "Wilson", "Anderson", "Thomas",
    "Taylor", "Moore", "Jackson", "Martin", "Lee", "Perez", "Thompson", "White",
    "Harris", "Sanchez", "Clark", "Ramirez", "Lewis", "Robinson", "Walker", "Young",
    "Allen", "King", "Wright", "Scott", "Torres", "Nguyen", "Hill", "Flores",
    "Green", "Adams", "Nelson", "Baker", "Hall", "Rivera", "Campbell", "Mitchell",
    "Carter", "Roberts", "Gomez", "Phillips", "Evans", "Turner", "Diaz", "Parker",
    "Cruz", "Edwards", "Collins", "Reyes", "Stewart", "Morris", "Morales", "Murphy",
    "Cook", "Rogers", "Gutierrez", "Ortiz", "Morgan", "Cooper", "Peterson", "Bailey",
    "Reed", "Kelly", "Howard", "Ramos", "Kim", "Cox", "Ward", "Richardson",
    "Watson", "Brooks", "Chavez", "Wood", "James", "Bennett", "Gray", "Mendoza",
    "Ruiz", "Hughes", "Price", "Alvarez", "Castillo", "Sanders", "Patel", "Myers",
    "Long", "Ross", "Foster", "Jimenez", "Powell", "Jenkins", "Perry", "Russell",
    "Sullivan", "Bell", "Coleman", "Butler", "Henderson", "Barnes", "Gonzales", "Fisher"
};

int first_names_count = sizeof(first_names) / sizeof(first_names[0]);
int surnames_count = sizeof(surnames) / sizeof(surnames[0]);

int main(int argc, char **argv)
{
  int n = atoi(argv[1]);

  Person **a = malloc(sizeof(Person*) * n);

  for (int i = 0; i < n; i++) {
    Person *p = malloc(sizeof(Person));
    p->id = n - i;
    p->name = first_names[i % first_names_count];
    p->surname = surnames[i % surnames_count];
    p->age = i;
    a[i] = p;
  }

  double start, end, cpu_time_used;
  start = clock();

  Person *t;
  int i, j = n, s = 1;
  while (s) {
    s = 0;
    for (i = 1; i < j; i++) {
      if (a[i]->id < a[i - 1]->id) {
        t = a[i];
        a[i] = a[i - 1];
        a[i - 1] = t;
        s = 1;
      }
    }
    j--;
  }

  end = clock();

  cpu_time_used = ((double) end - start) / CLOCKS_PER_SEC;

  printf("%.6f\n", cpu_time_used);

  return 0;
}
