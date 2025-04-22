#ifndef _LINREGR_H
#define _LINREGR_H

// Simple Linear Regression Calculator - George Maier 2020
// https://github.com/georgemaier/simple-linear-regression/blob/master/slr.cpp

#include <iostream>
#include <vector>
#include <cmath>

class slr
{
private:
    double WN1 = 0, WN2 = 0, WN3 = 0, WN4 = 0, Sy = 0, Sx = 0;

public:
    double r = 0, rsquared = 0, alpha = 0, beta = 0, x = 0, y = 0;
    double yhat = 0, ybar = 0, xbar = 0;
    double SSR = 0, SSE = 0, SST = 0;
    double residualSE = 0, residualmax = 0, residualmin = 0, residualmean = 0, t = 0;
    double SEBeta = 0;

    std::vector<double> residuals;
    int sample = 0;

    slr(std::vector<double> datax, std::vector<double> datay)
    {
        // This is the main regression function that is called when a new SLR object is created.
        if (datax.size() != datay.size())
            throw std::runtime_error("[ERROR] Lin regression x and y sizes must be equal");
        int samplesize = datax.size();

        // Calculate means
        auto slr_mean = [](std::vector<double> &data)
        {
            double x = 0;
            for (int n = 0; n < data.size(); n++)
            {
                x = x + data[n];
            }
            x = x / data.size();
            return x;
        };
        sample = samplesize;
        xbar = slr_mean(datax);
        ybar = slr_mean(datay);

        // Calculate r correlation
        for (int n = 0; n < samplesize; ++n)
        {
            WN1 += (datax[n] - xbar) * (datay[n] - ybar);
            WN2 += pow((datax[n] - xbar), 2);
            WN3 += pow((datay[n] - ybar), 2);
        }
        WN4 = WN2 * WN3;
        r = WN1 / (sqrt(WN4));

        // Calculate alpha and beta
        Sy = sqrt(WN3 / (samplesize - 1));
        Sx = sqrt(WN2 / (samplesize - 1));
        beta = r * (Sy / Sx);
        alpha = ybar - beta * xbar;

        // Calculate SSR, SSE, R-Squared, residuals
        // residuals[samplesize];
        for (int n = 0; n < samplesize; n++)
        {
            yhat = alpha + (beta * datax[n]);
            SSE += pow((yhat - ybar), 2);
            SSR += pow((datay[n] - yhat), 2);
            residuals.emplace_back(datay[n] - yhat);
            if (residuals[n] > residualmax)
            {
                residualmax = residuals[n];
            }
            if (residuals[n] < residualmin)
            {
                residualmin = residuals[n];
            }
            residualmean += fabs(residuals[n]);
        }
        residualmean = (residualmean / samplesize);
        SST = SSR + SSE;
        rsquared = SSE / SST; // Can also be obtained by r ^ 2 for simple regression (i.e. 1 independent variable)

        // Calculate T-test for Beta
        residualSE = sqrtf(SSR / (samplesize - 2));
        SEBeta = (residualSE / (Sx * sqrt(samplesize - 1)));
        t = beta / SEBeta;
    }

    void print()
    {
        // Print Results
        std::cout << "Data descriptions -----------------------------------------------------------------\n";
        std::cout << "Sample Size: " << this->sample << "     X mean: " << this->xbar << ";      Y mean: " << this->ybar << "\n";
        std::cout << "Pearson correlation (r): " << this->r << "\n\n";
        std::cout << "Regression Model  -----------------------------------------------------------------\n";
        std::cout << "R squared:           " << this->rsquared << "\n";
        std::cout << "Alpha Coefficient (q):   " << this->alpha << "\n";
        std::cout << "Beta Coefficient (m):    " << this->beta << "     Std. Error (B):  " << this->SEBeta << "     t-value:   " << this->t << "\n\n";
        std::cout << "Residuals         -----------------------------------------------------------------\n";
        std::cout << "Residual Std. Error: " << this->residualSE << " on " << (this->sample - 2) << " degrees of freedom\n";
        std::cout << "Mean Residual Dist.: " << this->residualmean << "\n";
        std::cout << "Max. Residual:       " << this->residualmax << "\n";
        std::cout << "Min. Residual:       " << this->residualmin << "\n \n";
        std::cout << "Additional Stats  -----------------------------------------------------------------\n";
        std::cout << "SSResiduals          " << this->SSR << "\n";
        std::cout << "SSExplained          " << this->SSE << "\n";
        std::cout << "SSTotal              " << this->SST << "\n\n";
    }
};
#endif //_LINREGR_H