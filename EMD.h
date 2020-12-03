#pragma once

static double* spline1(double x[], double y[], int n,
    double yp1, double ypn)
{
    double* uk, aaa, sig1, p, bbb, ccc, qn, un;
    static double y2[10240];
    uk = new double[n];
    int i, k;
    if (yp1 > 9.9e+29)
    {
        y2[0] = 0;
        uk[0] = 0;
    }
    else
    {
        y2[0] = -0.5;
        aaa = (y[1] - y[0]) / (x[1] - x[0]);
        uk[1] = (3.0 / (x[1] - x[0])) * (aaa - yp1);
    }
    for (i = 1; i <= n - 1; i++)
    {
        sig1 = (x[i] - x[i - 1]) / (x[i + 1] - x[i - 1]);
        p = sig1 * y2[i - 1] + 2.0;
        y2[i] = (sig1 - 1.0) / p;
        aaa = (y[i + 1] - y[i]) / (x[i + 1] - x[i]);
        bbb = (y[i] - y[i - 1]) / (x[i] - x[i - 1]);
        ccc = x[i + 1] - x[i - 1];
        uk[i] = (6.0 * (aaa - bbb) / ccc - sig1 * uk[i - 1]) / p;
    }
    if (ypn > 9.9e+29)
    {
        qn = 0.0;
        un = 0.0;
    }
    else
    {
        qn = 0.5;
        aaa = ypn - (y[n] - y[n - 1]) / (x[n] - x[n - 1]);
        un = (3.0 / (x[n] - x[n - 1])) * aaa;
    }
    y2[n] = (un - qn * uk[n - 1]) / (qn * y2[n - 1] + 1.0);
    for (k = n - 1; k >= 1; k--)
        y2[k] = y2[k] * y2[k + 1] + uk[k];
    delete[] uk;
    return y2;
}

static double spline2(double xa[], double ya[], double y2a[],
    int n, double& x)
{
    int klo, khi, k;
    double h, a, b, aaa, bbb, y;
    klo = 1;
    khi = n;
loop:   
    if (khi - klo > 1)
    {
        k = (khi + klo) / 2;
        if (xa[k] > x)
        {
            khi = k;
        }
        else
        {
            klo = k;
        }
        goto loop;
    }
    h = xa[khi] - xa[klo];
    if (h == 0)
    {
        //	AfxMessageBox("输入的xa[]数组有问题");
        return 0;
    }
    a = (xa[khi] - x) / h;
    b = (x - xa[klo]) / h;
    aaa = a * ya[klo] + b * ya[khi];
    bbb = (a * a * a - a) * y2a[klo] + (b * b * b - b) * y2a[khi];
    y = aaa + bbb * h * h / 6.0;
    return y;
}