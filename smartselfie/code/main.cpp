/*
====================================================================
 SmartSelfie - Face Detection and Image Processing with OpenCV
====================================================================

This program captures a snapshot from the webcam, detects faces
in the captured image, and performs basic image processing.

🧠 Features:
- Opens the webcam and allows the user to preview the feed
- Captures a snapshot when the user presses SPACE
- Saves the raw image as "snapshot_raw.png"
- Detects faces using Haar cascades and draws rectangles
- Saves the detected image as "snapshot_detected.png"
- Applies grayscale conversion and Gaussian blur to the raw image
- Saves the blurred image as "snapshot_blur.png"
- Displays all resulting images step-by-step for visualization

📂 Files created:
- snapshot_raw.png         → original captured photo
- snapshot_detected.png    → photo with detected face(s) outlined
- snapshot_blur.png        → grayscale + blurred version of raw image

Requirements:
- OpenCV (tested with 4.x)
- Haar cascade XML file ("haarcascade_frontalface_default.xml")
  must be in the same folder as the executable

Author: Vaggelis Papaioannou
====================================================================
*/


#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Global cascade for face detection
// It loads a pre-trained Haar cascade XML file that defines the features of a human face.
// This classifier is applied to grayscale images to find face regions.
CascadeClassifier faceCascade;

// Open the webcam
// Attempts to open the default webcam (device 0).
// Returns true if successful, false otherwise.
// Displays an error message if the webcam cannot be accessed.
bool openWebcam(VideoCapture& cap) {
    cap.open(0);
    if (!cap.isOpened()) {
        cerr << "Error: Cannot open webcam." << endl;
        return false;
    }
    return true;
}

// Loads the Haar cascade XML file used for face detection.
// The default path is "haarcascade_frontalface_default.xml", which must be present in the executable directory.
// Returns true if the cascade file is successfully loaded, otherwise prints an error and returns false.
bool loadFaceCascade(const string& path = "haarcascade_frontalface_default.xml") {
    if (!faceCascade.load(path)) {
        cerr << "Error: Could not load Haar cascade file: " << path << endl;
        return false;
    }
    return true;
}

// Apply face detection and draw rectangles
// Detects faces in the given frame using the loaded Haar cascade classifier.
// Steps:
// 1. Converts the input frame to grayscale.
// 2. Applies histogram equalization to improve contrast.
// 3. Uses faceCascade to detect faces with `detectMultiScale`.
// 4. Draws blue rectangles around each detected face directly on the input frame.
// Returns a vector of detected face rectangles (cv::Rect).
vector<Rect> detectFaces(Mat& frame) {
    vector<Rect> faces;
    Mat gray;

    cvtColor(frame, gray, COLOR_BGR2GRAY);
    equalizeHist(gray, gray);
    faceCascade.detectMultiScale(gray, faces, 1.1, 3, 0, Size(30, 30));

    for (const Rect& face : faces) {
        rectangle(frame, face, Scalar(255, 0, 0), 2);
    }
    return faces;
}

// Captures a single frame from the given VideoCapture object (webcam).
// - Attempts to read the next available frame.
// - If successful, stores it in the provided `frame` reference.
// - If the frame is empty (e.g., webcam not working), prints an error and returns false.
// Returns true if the frame was successfully captured, false otherwise.
bool captureFrame(VideoCapture& cap, Mat& frame) {
    cap >> frame;
    if (frame.empty()) {
        cerr << "Error: Could not read frame from webcam." << endl;
        return false;
    }
    return true;
}

// Saves the raw captured frame as "snapshot_raw.png" and displays it.
// - Takes the input frame (rawFrame), saves it to disk.
// - Displays the saved image in a window called "Raw Snapshot".
// - Waits for a key press before closing the window.
// - Returns a cloned copy of the raw image for further processing.
Mat processAndSaveImages(const Mat& rawFrame) {
    string rawFilename = "snapshot_raw.png";
    imwrite(rawFilename, rawFrame);
    cout << "Saved raw photo as " << rawFilename << endl;

    imshow("Raw Snapshot", rawFrame);
    waitKey(0);
    destroyWindow("Raw Snapshot");

    return rawFrame.clone();  // return raw image for later use
}

// Applies face detection on the given image and saves the result.
// - Clones the input image to preserve the original.
// - Detects faces using the Haar cascade and draws rectangles around them.
// - If no faces are found, prints a message.
// - Saves the processed image as "snapshot_detected.png".
// - Displays the detected snapshot in a window and waits for a key press before closing it.
void detectAndShow(const Mat& inputImage) {
    Mat detectedFrame = inputImage.clone();
    vector<Rect> faces = detectFaces(detectedFrame);

    if (faces.empty()) {
        cout << "No faces detected." << endl;
    } else {
        cout << "Detected " << faces.size() << " face(s)." << endl;
    }

    // Save and show
    string detectedFilename = "snapshot_detected.png";
    imwrite(detectedFilename, detectedFrame);
    cout << "Saved detected photo as " << detectedFilename << endl;

    imshow("Detected Snapshot", detectedFrame);
    waitKey(0);
    destroyWindow("Detected Snapshot");
}


// Applies grayscale conversion and Gaussian blur to the captured raw image.
// - Converts the original image to grayscale.
// - Applies a Gaussian blur with a 9x9 kernel.
// - Saves the blurred image as "snapshot_blur.png".
// - Displays both the original raw image and the blurred grayscale version.
// - Waits for a key press and then closes all windows.
void applyGaussianBlurToRaw(const Mat& rawImage) {
    // Convert to grayscale
    Mat grayImage;
    cvtColor(rawImage, grayImage, COLOR_BGR2GRAY);

    // Apply Gaussian blur
    Mat blurredImage;
    GaussianBlur(grayImage, blurredImage, Size(9, 9), 0);

    // Save and show
    string blurredFile = "snapshot_blur.png";
    imwrite(blurredFile, blurredImage);
    cout << "Saved blurred image: " << blurredFile << endl;

    imshow("Original Snapshot", rawImage);
    imshow("Blurred Grayscale", blurredImage);
    waitKey(0);
    destroyAllWindows();
}

// Main entry point of the program.
// - Initializes the webcam and loads the face detection model.
// - Continuously waits for the user to capture a frame from the webcam.
// - Once a frame is captured:
//     1. Saves and displays the raw image.
//     2. Detects and displays faces on a copy of the image.
//     3. Applies grayscale and Gaussian blur to the raw image and displays the result.
// - The loop exits after one successful capture and processing sequence.
// - Releases the webcam and exits cleanly.
int main() {
    VideoCapture cap;
    if (!openWebcam(cap)) return -1;
    if (!loadFaceCascade()) return -1;

    Mat frame, rawImage;

    while (true) {
        if (!captureFrame(cap, frame)) {
            cout << "Cancelled by user.\n";
            return 0;
        }

        rawImage = processAndSaveImages(frame);
        detectAndShow(rawImage);
        applyGaussianBlurToRaw(rawImage);
        break;
    }

    cap.release();
    return 0;
}
