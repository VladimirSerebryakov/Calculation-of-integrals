#include <math.h>
#include <stdio.h>
#include <getopt.h>

typedef double afunc (double);
double root(afunc* f, afunc* g, double a, double b, double eps1);
double integral(afunc* f, double a, double b, double eps2);

extern double f1(double x);
extern double f2(double x);
extern double f3(double x);
extern double test_f1(double x);
extern double test_f2(double x);
extern double test_f3(double x);

int cnt = 0; // A variable for determining the number of iterations

// Finding the intersection point of functions f and g with a precision of eps1.
double root(afunc* f, afunc* g, double a, double b, double eps1) {
   double c, c_old;
   double Fa, Fb, Fc; // F(a) = f(a) - g(a) ...
   double Fce; // F(c + e) or F(c - e)
   int left = 0; // A flag indicating the direction of approach
   Fa = f(a) - g(a);
   Fb = f(b) - g(b);
   c = (a * Fb - b * Fa) / (Fb - Fa);
   double mid = (a + b) / 2;
   double Fmid = f(mid) - g(mid);

   // Verifying that functions f' and f'' have the same sign.
   if ((Fmid > (Fa + Fb)/2 && Fa > 0) || (Fmid < (Fa + Fb)/2 && Fa < 0)) {
      left = 1;
   }
   c_old = c + 10.0 * eps1;

   while (1) {
      if (fabs(c - c_old) < eps1) {
         return c;
      }
      Fb = f(b) - g(b);
      Fa = f(a) - g(a);
      Fc = f(c) - g(c);
      c_old = c;
      if (left) {
         Fce = f(c + eps1) - g(c + eps1);
         if (Fce * Fc < 0) {
            return c;
         }
         a = c;
         Fa = f(a) - g(a);
         c = (a * Fb - b * Fa) / (Fb - Fa);
      }else {
         Fce = f(c - eps1) - g(c - eps1);
         if (Fce * Fc < 0) {
            return c;
         }
         b = c;
         Fb = f(b) - g(b);
         c = (a * Fb - b * Fa) / (Fb - Fa);
      }
      cnt += 1;
   }
}

// The integral function operates using the rectangle formula with error tolerance eps2
double integral(afunc* f, double a, double b, double eps2) {
   double In = 0;
   double I2n = 0;
   int n = 10;
   double h = (b - a)/n;
   for (int i = 0; i < n; i++) {
      In += f(a + (i + 0.5) * h);
   }
   In = In * h;
   n = n * 2;
   h = (b - a)/n;
   for (int i = 0; i < n; i++) {
      I2n += f(a + (i + 0.5) * h);
   }
   I2n = I2n * h;
   while (fabs(In - I2n) / 3 > eps2) {
      In = I2n;
      n *= 2;
      I2n = 0;
      h = (b - a) / n;
      for (int i = 0; i < n; i++) {
         I2n += f(a + (i + 0.5) * h);
      }
      I2n = I2n * h;
   }
   return I2n;
}

int main(int argc, char* argv[]) {
   double x1, x2, x3;

   if (argc == 1) {
      x1 = root(f1, f3, -1.95, -1.0, 1e-6);
      x2 = root(f3, f2, -1.0, 0.0, 1e-6);
      x3 = root(f1, f2, 0.0, 2.0, 1e-6);
      printf("answer: %f", integral(f1, x1, x3, 1e-6) - integral(f2, x2, x3, 1e-6) - integral(f3, x1, x2, 1e-6));
   }

   static struct option long_option[] = {
      {"help", 0, 0, 'h'},
      {"root", 0, 0, 'r'},
      {"iterations", 0, 0, 'i'},
      {"test-root", 1, 0, 'R'},
      {"test-integral", 1, 0, 'I'}
   };
   int opt;
   while ((opt = getopt_long(argc, argv, "hriR:I:", long_option, 0)) != -1) {
      switch (opt) {
         case 'h':
            printf("All available options:\n");
            printf("--help, -h Reference.\n");
            printf("--root, -r Output of Curve Intersection Points.\n");
            printf("--iterations, -i Outputting the number of iterations required to find the intersection points.\n");
            printf("--test-root, -R For testing the root function. In the format F1:F2:A:B:E:R.\n");
            printf("--test-integral, -I For testing the integral function. In the format F:A:B:E:R.\n");
            break;
         case 'r':
            x1 = root(f1, f3, -1.95, -1.0, 1e-6);
            x2 = root(f3, f2, -1.0, 0.0, 1e-6);
            x3 = root(f1, f2, 0.0, 2.0, 1e-6);
            printf("Points of Intersection: %f %f %f\n", x1, x2, x3);
            break;
         case 'i':
            cnt = 0;
            x1 = root(f1, f3, -1.95, -1.0, 1e-6);
            x2 = root(f3, f2, -1.0, 0.0, 1e-6);
            x3 = root(f1, f2, 0.0, 2.0, 1e-6);
            printf("Number of iterations for finding intersection points: %d\n", cnt);
            break;
         case 'R': {
            int fun1, fun2;
            double A, B, E, R;
            if (sscanf(optarg, "%d:%d:%lf:%lf:%lf:%lf", &fun1, &fun2, &A, &B, &E, &R) == 6) {
               afunc* func1;
               afunc* func2;
               int flag = 1;
               if (fun1 == 1) {
                  func1 = f1;
               }else if (fun1 == 2) {
                  func1 = f2;
               }else if (fun1 == 3) {
                  func1 = f3;
               }else if (fun1 == 4) {
                  func1 = test_f1;
               } else if (fun1 == 5) {
                  func1 = test_f2;
               } else if (fun1 == 6) {
                  func1 = test_f3;
               } else {
                  flag = 0;
               }
               if (fun2 == 1) {
                  func2 = f1;
               } else if (fun2 == 2) {
                  func2 = f2;
               } else if (fun2 == 3) {
                  func2 = f3;
               } else if (fun2 == 4){
                  func2 = test_f1;
               } else if (fun2 == 5) {
                  func2 = test_f2;
               } else if (fun2 == 6) {
                  func2 = test_f3;
               } else {
                  flag = 0;
               }
               if (flag) {
                  double ans = root(func1, func2, A, B, E);
                  double error = fabs(ans - R);
                  double error2;
                  if (R > 1e-9) {
                     error2 = fabs(error / R);
                  }else {
                     error2 = 0.0;
                  }
                  printf("%f %f %f\n", ans, error, error2);
               }else {
                  printf("Wrong function id\n");
               }
            }else {
               printf("Wrong format\n");
            }
            break;
         }
         case 'I': {
            int fun;
            double A, B, E, R;
            if (sscanf(optarg, "%d:%lf:%lf:%lf:%lf", &fun, &A, &B, &E, &R) == 5) {
               afunc* func;
               int flag = 1;
               if (fun == 1) {
                  func = f1;
               } else if (fun == 2) {
                  func = f2;
               } else if (fun == 3) {
                  func = f3;
               } else if (fun == 4) {
                  func = test_f1;
               } else if (fun == 5) {
                  func = test_f2;
               } else if (fun == 6) {
                  func = test_f3;
               } else {
                  flag = 0;
               }
               if (flag) {
                  double ans = integral(func, A, B, E);
                  double error = fabs(ans - R);
                  double error2;
                  if (R >= 1e-9) {
                     error2 = fabs(error / R);
                  }else {
                     error2 = 0.0;
                  }
                  printf("%f %f %f\n", ans, error, error2);
               }else {
                  printf("Wrong function id\n");
               }
            }else {
               printf("Wrong format\n");
            }
            break;
         }
         default:
            return -1;
      }
   }

   return 0;
}
