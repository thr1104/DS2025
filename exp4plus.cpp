#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <iomanip>
#include <cmath>
using namespace std;

struct BBox {
    float x1, y1, x2, y2, score;
    BBox() : x1(0), y1(0), x2(0), y2(0), score(0.0f) {}
    BBox(float x1, float y1, float x2, float y2, float score)
        : x1(x1), y1(y1), x2(x2), y2(y2), score(score) {}
    float area() const { return (x2 - x1) * (y2 - y1); }
    static float intersectionArea(const BBox& a, const BBox& b) {
        float inter_x1 = (a.x1 > b.x1) ? a.x1 : b.x1;
        float inter_y1 = (a.y1 > b.y1) ? a.y1 : b.y1;
        float inter_x2 = (a.x2 < b.x2) ? a.x2 : b.x2;
        float inter_y2 = (a.y2 < b.y2) ? a.y2 : b.y2;
        if (inter_x2 <= inter_x1 || inter_y2 <= inter_y1) return 0.0f;
        return (inter_x2 - inter_x1) * (inter_y2 - inter_y1);
    }
    static float iou(const BBox& a, const BBox& b) {
        float inter = intersectionArea(a, b);
        float union_area = a.area() + b.area() - inter;
        return (union_area > 0) ? inter / union_area : 0.0f;
    }
    bool operator<(const BBox& other) const { return score > other.score; }
};

int partition(vector<BBox>& arr, int low, int high) {
    BBox pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j] < pivot) {
            ++i;
            BBox temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    BBox temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return i + 1;
}

void quickSort(vector<BBox>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void merge(vector<BBox>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<BBox> L(n1);
    vector<BBox> R(n2);
    for (int i = 0; i < n1; ++i) L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j) R[j] = arr[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] < R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(vector<BBox>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void heapify(vector<BBox>& arr, int n, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < n && arr[l] < arr[largest]) largest = l;
    if (r < n && arr[r] < arr[largest]) largest = r;
    if (largest != i) {
        BBox temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        heapify(arr, n, largest);
    }
}

void heapSort(vector<BBox>& arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; --i)
        heapify(arr, n, i);
    for (int i = n - 1; i > 0; --i) {
        BBox temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        heapify(arr, i, 0);
    }
}

void insertionSort(vector<BBox>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; ++i) {
        BBox key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] < key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

void bubbleSort(vector<BBox>& arr) {
    int n = arr.size();
    bool swapped;
    for (int i = 0; i < n - 1; ++i) {
        swapped = false;
        for (int j = 0; j < n - i - 1; ++j) {
            if (!(arr[j] < arr[j + 1])) {
                BBox temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

vector<BBox> nms(const vector<BBox>& boxes, float iou_threshold) {
    if (boxes.empty()) return vector<BBox>();
    vector<BBox> selected;
    for (unsigned int i = 0; i < boxes.size(); ++i) {
        bool suppress = false;
        for (unsigned int j = 0; j < selected.size(); ++j) {
            if (BBox::iou(boxes[i], selected[j]) > iou_threshold) {
                suppress = true;
                break;
            }
        }
        if (!suppress) {
            selected.push_back(boxes[i]);
        }
    }
    return selected;
}

vector<BBox> generateRandomBoxes(int count, float w, float h) {
    vector<BBox> boxes;
    srand(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < count; ++i) {
        float x1 = static_cast<float>(rand()) / RAND_MAX * w * 0.8f;
        float y1 = static_cast<float>(rand()) / RAND_MAX * h * 0.8f;
        float w_ = (static_cast<float>(rand()) / RAND_MAX * 0.3f + 0.1f) * w;
        float h_ = (static_cast<float>(rand()) / RAND_MAX * 0.3f + 0.1f) * h;
        float x2 = (x1 + w_ > w) ? w : x1 + w_;
        float y2 = (y1 + h_ > h) ? h : y1 + h_;
        float score = static_cast<float>(rand()) / RAND_MAX * 0.5f + 0.5f;
        boxes.push_back(BBox(x1, y1, x2, y2, score));
    }
    return boxes;
}

vector<BBox> generateClusteredBoxes(int count, float w, float h) {
    vector<BBox> boxes;
    srand(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < count; ++i) {
        float cx = (static_cast<float>(rand()) / RAND_MAX * 0.6f + 0.2f) * w;
        float cy = (static_cast<float>(rand()) / RAND_MAX * 0.6f + 0.2f) * h;
        float dx = (static_cast<float>(rand()) / RAND_MAX * 0.2f - 0.1f) * w;
        float dy = (static_cast<float>(rand()) / RAND_MAX * 0.2f - 0.1f) * h;
        float w_ = (static_cast<float>(rand()) / RAND_MAX * 0.1f + 0.05f) * w;
        float h_ = (static_cast<float>(rand()) / RAND_MAX * 0.1f + 0.05f) * h;
        float x1 = (cx + dx - w_/2 < 0) ? 0 : cx + dx - w_/2;
        float y1 = (cy + dy - h_/2 < 0) ? 0 : cy + dy - h_/2;
        float x2 = (cx + dx + w_/2 > w) ? w : cx + dx + w_/2;
        float y2 = (cy + dy + h_/2 > h) ? h : cy + dy + h_/2;
        float score = static_cast<float>(rand()) / RAND_MAX * 0.4f + 0.6f;
        boxes.push_back(BBox(x1, y1, x2, y2, score));
    }
    return boxes;
}

double timeSort(void (*sortFn)(vector<BBox>&, int, int), vector<BBox> data, int low, int high) {
    clock_t start = clock();
    sortFn(data, low, high);
    clock_t end = clock();
    return static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000.0;
}

double timeSortSimple(void (*sortFn)(vector<BBox>&), vector<BBox> data) {
    clock_t start = clock();
    sortFn(data);
    clock_t end = clock();
    return static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000.0;
}

int main() {
    cout << "代码作业：排序 ===" << endl;
    cout << "目标：实现多种排序算法并应用于NMS，分析性能" << endl << endl;

    int sizes[] = {100, 500, 1000, 5000, 10000};
    string distNames[] = {"Random", "Clustered"};

    cout << setw(10) << "Size"
         << setw(12) << "Dist"
         << setw(15) << "Algorithm"
         << setw(12) << "Time(μs)" << endl;
    cout << string(55, '-') << endl;

    for (int s = 0; s < 5; ++s) {
        int n = sizes[s];
        for (int d = 0; d < 2; ++d) {
            vector<BBox> original_data = (d == 0) ? generateRandomBoxes(n, 1000.0f, 1000.0f) : generateClusteredBoxes(n, 1000.0f, 1000.0f);

            // QuickSort
            double t = timeSort(quickSort, original_data, 0, n - 1);
            cout << setw(10) << n << setw(12) << distNames[d] << setw(15) << "QuickSort" << setw(12) << static_cast<long long>(t) << endl;

            // MergeSort
            t = timeSort(mergeSort, original_data, 0, n - 1);
            cout << setw(10) << n << setw(12) << distNames[d] << setw(15) << "MergeSort" << setw(12) << static_cast<long long>(t) << endl;

            // HeapSort
            t = timeSortSimple(heapSort, original_data);
            cout << setw(10) << n << setw(12) << distNames[d] << setw(15) << "HeapSort" << setw(12) << static_cast<long long>(t) << endl;
            
			//BubbleSort    
            t = timeSortSimple(bubbleSort, original_data);
            cout << setw(10) << n << setw(12) << distNames[d] << setw(15) << "BubbleSort" << setw(12) << static_cast<long long>(t) << endl;

            // InsertionSort 
            t = timeSortSimple(insertionSort, original_data);
            cout << setw(10) << n << setw(12) << distNames[d] << setw(15) << "InsertionSort" << setw(12) << static_cast<long long>(t) << endl;

        }
        cout << endl;
    }

    {
        vector<BBox> boxes = generateRandomBoxes(1000, 1000.0f, 1000.0f);
        vector<BBox> sorted_boxes = boxes;
        quickSort(sorted_boxes, 0, sorted_boxes.size() - 1);
        clock_t start = clock();
        vector<BBox> result = nms(sorted_boxes, 0.5f);
        clock_t end = clock();
        double nms_time = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000.0;
        cout << "Example: NMS on 1000 random boxes -> kept " << result.size()
             << " boxes in " << static_cast<long long>(nms_time) << " μs." << endl;
    }

    return 0;
}
