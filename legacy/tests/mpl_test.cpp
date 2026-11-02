
#include <thread>
#define WITHOUT_NUMPY
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

int main() {
    // Define the number of points
    int num_points = 100;

    // Create vectors to hold the x and y coordinates
    std::vector<double> x(num_points);
    std::vector<double> y(num_points);

    // Generate points for the semicircle
    for (int i = 0; i < num_points; ++i) {
        double theta = M_PI * i / (num_points - 1); // Angle from 0 to π
        x[i] = cos(theta); // x = cos(theta)
        y[i] = sin(theta); // y = sin(theta)
    }

    // Fill the semicircle
    plt::fill(x, y, {{"color", "#0000FF33"}}); // Fill with blue color
    plt::plot(x, y);

    // Set the aspect of the plot to be equal
    plt::axis("equal");

    // Set labels and title
    plt::xlabel("X-axis");
    plt::ylabel("Y-axis");
    plt::title("Filled Semicircle");

    // Show the plot
    plt::show();

    return 0;
}