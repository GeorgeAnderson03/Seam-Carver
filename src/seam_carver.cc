#include "seam_carver.hpp"

// implement the rest of SeamCarver's functions here

// given functions below, DO NOT MODIFY
const ImagePPM& SeamCarver::GetImage() const { return image_; }

int SeamCarver::GetHeight() const { return height_; }

int SeamCarver::GetWidth() const { return width_; }

SeamCarver::SeamCarver(const ImagePPM& image): image_(image) {
  height_ = image.GetHeight();
  width_ = image.GetWidth();
}

void SeamCarver::SetImage(const ImagePPM& image) {
  image_ = image;
  width_ = image.GetWidth();
  height_ = image.GetHeight();
}

int SeamCarver::GetEnergy(int row, int col) const {
  int above = row - 1;
  int below = row + 1;
  int left = col - 1;
  int right = col + 1;
  int rrow = 0;
  int grow = 0;
  int brow = 0;
  int rcol = 0;
  int gcol = 0;
  int bcol = 0;
  int finalr = 0;
  int finalc = 0;
  int finalenergy = 0;
  if (above < 0) {
    above = height_ - 1;
  }
  if (below >= height_) {
    below = 0;
  }
  rrow = image_.GetPixel(above, col).GetRed() -
         image_.GetPixel(below, col).GetRed();
  grow = image_.GetPixel(above, col).GetGreen() -
         image_.GetPixel(below, col).GetGreen();
  brow = image_.GetPixel(above, col).GetBlue() -
         image_.GetPixel(below, col).GetBlue();
  finalr = (rrow * rrow) + (grow * grow) + (brow * brow);
  if (right >= width_) {
    right = 0;
  }
  if (left < 0) {
    left = width_ - 1;
  }
  rcol = image_.GetPixel(row, left).GetRed() -
         image_.GetPixel(row, right).GetRed();
  gcol = image_.GetPixel(row, left).GetGreen() -
         image_.GetPixel(row, right).GetGreen();
  bcol = image_.GetPixel(row, left).GetBlue() -
         image_.GetPixel(row, right).GetBlue();
  finalc = (rcol * rcol) + (gcol * gcol) + (bcol * bcol);
  finalenergy = finalc + finalr;
  return finalenergy;
}
int* SeamCarver::GetHorizontalSeam() const {
  int* random = GetVerticalSeam();
  return random;
}

int* SeamCarver::GetVerticalSeam() const {
  int** d2array = new int*[height_];
  int row = height_;
  int col = width_;
  int leftbelow = 0;
  int below = 0;
  int rightbelow = 0;
  for (int j = 0; j < row; j++) {
    d2array[j] = new int[col];
  }
  for (int row = 0; row < height_; row++) {
    for (int col = 0; col < width_; col++) {
      d2array[row][col] = GetEnergy(row, col);
    }
  }
  for (int row = height_ - 1; row > 0; row--) {
    int sloweri = row;
    for (int col = 0; col < width_; col++) {
      if (!(sloweri == height_ - 1)) {
        if (col == 0) {
          below = d2array[sloweri - 1][col];
          rightbelow = d2array[sloweri - 1][col + 1];
          if (rightbelow < below) {
            d2array[sloweri][col] += rightbelow;
          } else {
            d2array[sloweri][col] += below;
          }
        } else if (col == width_ - 1) {
          below = d2array[sloweri - 1][col];
          leftbelow = d2array[sloweri - 1][col - 1];
          if (leftbelow < below) {
            d2array[sloweri][col] += leftbelow;
          } else {
            d2array[sloweri][col] += below;
          }
        } else {
          below = d2array[sloweri - 1][col];
          leftbelow = d2array[sloweri - 1][col - 1];
          rightbelow = d2array[sloweri - 1][col + 1];
          if (below < rightbelow && below < leftbelow) {
            d2array[sloweri][col] += below;
          } else if (leftbelow < below && leftbelow < rightbelow) {
            d2array[sloweri][col] += leftbelow;
          } else if (rightbelow < below && rightbelow < leftbelow) {
            d2array[sloweri][col] += rightbelow;
          } else if (leftbelow == rightbelow && leftbelow < below) {
            d2array[sloweri][col] += leftbelow;
          } else if (leftbelow == below || rightbelow == below) {
            d2array[sloweri][col] += below;
          }
        }
      }
    }
  }
  int* bestpath = new int[height_];
  int min = INT32_MAX;
  int smallcol = 0;
  for (int col = 0; col < width_; col++) {
    if (d2array[0][col] < min) {
      min = d2array[0][col];
      smallcol = col;
    }
  }
  bestpath[0] = smallcol;
  int leftbelow1 = 0;
  int below1 = 0;
  int rightbelow1 = 0;
  for (int row = 1; row < height_; row++) {
    if (smallcol == 0) {
      below1 = d2array[row][smallcol];
      rightbelow1 = d2array[row][smallcol + 1];
      if (rightbelow1 < below1) {
        smallcol++;
        bestpath[row] = smallcol;
      } else {
        bestpath[row] = smallcol;
      }
    } else if (smallcol == width_ - 1) {
      below1 = d2array[row][smallcol];
      leftbelow1 = d2array[row][smallcol - 1];
      if (leftbelow1 < below1) {
        smallcol--;
        bestpath[row] = smallcol;
      } else {
        bestpath[row] = smallcol;
      }
    } else {
      below1 = d2array[row][smallcol];
      rightbelow1 = d2array[row][smallcol + 1];
      leftbelow1 = d2array[row][smallcol - 1];
      if (below1 < rightbelow1 && below1 < leftbelow1) {
        bestpath[row] = smallcol;
      } else if (leftbelow1 < below1 && leftbelow1 < rightbelow1) {
        smallcol--;
        bestpath[row] = smallcol;
      } else if (rightbelow1 < below1 && rightbelow1 < leftbelow1) {
        smallcol++;
        bestpath[row] = smallcol;
      } else if (leftbelow1 == rightbelow1 && leftbelow1 < below1) {
        smallcol--;
        bestpath[row] = smallcol;
      } else if (leftbelow1 == below1 || rightbelow1 == below1) {
        bestpath[row] = smallcol;
      }
    }
  }
  for (int i = 0; i < height_; i++) {
    delete[] d2array[i];
  }
  delete[] d2array;
  return bestpath;
}

void SeamCarver::RemoveHorizontalSeam() {}

void SeamCarver::RemoveVerticalSeam() {
  width_--;
  image_.RemoveVerticalHelper(GetVerticalSeam());
  // delete[] GetVerticalSeam();
}