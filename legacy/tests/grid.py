import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import json

def calculate_deviation_metrics(original_points, noisy_points):
    """
    Calculate deviation metrics between original and noisy points.

    Parameters:
    original_points (np.ndarray): Array of original points (N x 3).
    noisy_points (np.ndarray): Array of noisy points (N x 3).

    Returns:
    dict: A dictionary containing average displacement error, 
          root mean square error, and maximum displacement error for overall and each axis.
    """
    # Calculate the displacement vectors
    displacements = noisy_points - original_points
    
    # Calculate the Euclidean distances
    distances = np.linalg.norm(displacements, axis=1)
    
    round = lambda a: np.around(a, decimals=4)
    # Calculate overall metrics
    average_displacement_error = round(np.mean(distances))
    root_mean_square_error = round(np.sqrt(np.mean(distances**2)))
    maximum_displacement_error = round(np.max(distances))
    
    # Calculate metrics for each axis
    x_displacements = displacements[:, 0]
    y_displacements = displacements[:, 1]
    z_displacements = displacements[:, 2]
    
    
    metrics_per_axis = {
        'X': {
            'Average Displacement Error': round(np.mean(np.abs(x_displacements))),
            'Root Mean Square Error    ': round(np.sqrt(np.mean(x_displacements**2))),
            'Maximum Displacement Error': round(np.max(np.abs(x_displacements)))
        },
        'Y': {
            'Average Displacement Error': round(np.mean(np.abs(y_displacements))),
            'Root Mean Square Error    ': round(np.sqrt(np.mean(y_displacements**2))),
            'Maximum Displacement Error': round(np.max(np.abs(y_displacements)))
        },
        'Z': {
            'Average Displacement Error': round(np.mean(np.abs(z_displacements))),
            'Root Mean Square Error    ': round(np.sqrt(np.mean(z_displacements**2))),
            'Maximum Displacement Error': round(np.max(np.abs(z_displacements)))
        }
    }
    
    # Combine overall metrics and metrics per axis
    overall_metrics = {
        'Average Displacement Error': average_displacement_error,
        'Root Mean Square Error    ': root_mean_square_error,
        'Maximum Displacement Error': maximum_displacement_error,
        'Metrics Per Axis': metrics_per_axis
    }
    
    return overall_metrics

def gen_grid(num_points, space_size):

    # Create a grid of points
    x = np.linspace(0, space_size, num_points)
    y = np.linspace(0, space_size, num_points)
    z = np.linspace(0, space_size, num_points)
    X, Y, Z = np.meshgrid(x, y, z)

    # Flatten the grid points for plotting
    points = np.vstack([X.ravel(), Y.ravel(), Z.ravel()]).T

    # Generate noise
    noise_scale = 0.08  # Scale of noise
    noise = np.random.normal(0, noise_scale, points.shape)

    # Create noisy points
    noisy_points = points + noise
    
    return points, noisy_points

def read_grid(file_path):
    """
    Reads target and actual vectors from a CSV file using NumPy.

    Parameters:
    file_path (str): The path to the CSV file.

    Returns:
    tuple: A tuple containing two NumPy arrays (target_vector, actual_vector).
    """
    # Load the data from the CSV file
    data = np.genfromtxt(file_path, delimiter=',', skip_header=1)

    # Extract target and actual vectors
    target_vector = data[:, :3]  # First three columns for target
    actual_vector = data[:, 3:6]  # Last three columns for actual

    return target_vector, actual_vector

# points, noisy_points = gen_grid(5, 10.0)
points, noisy_points = read_grid('positions.csv')
metrics = calculate_deviation_metrics(points, noisy_points)
print(json.dumps(metrics, indent=4))

plot = True
if plot:
    # Plotting
    fig = plt.figure(figsize=(12, 6))

    # Original points
    ax1 = fig.add_subplot(121, projection='3d')
    ax1.scatter(points[:, 0], points[:, 1], points[:, 2], color='blue', label='Original Points')
    ax1.set_title('Original Points')
    ax1.set_xlabel('X (cm)')
    ax1.set_ylabel('Y (cm)')
    ax1.set_zlabel('Z (cm)')
    ax1.set_box_aspect([1,1,1])  # Equal aspect ratio
    ax1.legend()

    # Noisy points
    ax2 = fig.add_subplot(122, projection='3d')
    ax2.scatter(noisy_points[:, 0], noisy_points[:, 1], noisy_points[:, 2], color='red', label='Noisy Points')
    ax2.set_title('Noisy Points')
    ax2.set_xlabel('X (cm)')
    ax2.set_ylabel('Y (cm)')
    ax2.set_zlabel('Z (cm)')
    ax2.set_box_aspect([1,1,1])  # Equal aspect ratio
    ax2.legend()

    fig2 = plt.figure(figsize=(18, 6))

    # Original and noisy points in the same plot
    ax = fig2.add_subplot(111, projection='3d')
    ax.scatter(points[:, 0], points[:, 1], points[:, 2], color='blue', label='Target Points')
    ax.scatter(noisy_points[:, 0], noisy_points[:, 1], noisy_points[:, 2], color='red', label='Actual Points')

    # Add arrows from original points to noisy points
    for original, noisy in zip(points, noisy_points):
        ax.quiver(original[0], original[1], original[2], 
                noisy[0] - original[0], noisy[1] - original[1], noisy[2] - original[2], 
                color='green', arrow_length_ratio=0.2)

    ax.set_title('Target and Actual Points')
    ax.set_xlabel('X (cm)')
    ax.set_ylabel('Y (cm)')
    ax.set_zlabel('Z (cm)')
    ax.set_box_aspect([1, 1, 1])  # Equal aspect ratio
    ax.legend()

    plt.tight_layout()
    plt.show()
