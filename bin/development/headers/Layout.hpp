#pragma once

#include <string>
#include <vector>

// Used in GDSII
struct Units {
  double  user,
          physical;
};

struct Coord {
  int32_t x,
          y;
};

// Used in GDSII
struct Property {
  int16_t     index;
  std::string value;
};

enum class GeometryType {
  undefined = 0,
  polygon,
  path,
  text,
  rectangle,
  reference,
};

struct Geometry {
  GeometryType          type;
  int16_t               layer;
  Coord                 min,
                        max;
  std::vector<Property> properties;
  std::vector<Coord>    coords;
public:
  Geometry(GeometryType t) : type(t), layer(-1), min({ -1, -1 }), max({-1, -1}) {}
};

struct Polygon : public Geometry {
  int16_t               dataType;
public:
  Polygon() : Geometry(GeometryType::polygon), dataType(0) {}
};

struct Path : public Geometry {
  int16_t               dataType,
                        pathType;
  int32_t               width;
public:
  Path() : Geometry(GeometryType::path), dataType(0), pathType(0), width(0) {}
};

struct Text : public Geometry {
  int16_t                textType,
                         flagsPresentation,
                         pathType,
                         flagsTransformation;
  double                 magnification;
  int32_t                width;
  std::string            stringValue;
public:
  Text() : Geometry(GeometryType::text), textType(0), flagsPresentation(0), pathType(0), flagsTransformation(0), magnification(1.0), width(0) {}
};

struct Rectangle : public Geometry {
  int16_t                rectType;
public:
  Rectangle() : Geometry(GeometryType::rectangle), rectType(0) {}
};

struct Element;

struct Reference : public Geometry {
  std::string             name;
  Element                *referenceTo;
  int16_t                 transformationFlags;
  double                  magnification;
public:
  Reference() : Geometry(GeometryType::reference), name(""), referenceTo(nullptr), transformationFlags(0), magnification(1.0) {}
};

struct Element {
  bool                    isFlat;
  Coord                   min,
                          max;
  bool                    nested;
  std::string             name;
  std::vector<Geometry *> geometries;
public:
  Element() : isFlat(true), min({ -1, -1 }), max({-1, -1}), nested(false) {}
  ~Element() {
    for (size_t i = 0; i < geometries.size(); ++i) {
      delete geometries[i];
      geometries[i] = nullptr;
    }
    geometries.clear();
  }
};

struct Layer {
  int16_t                 layer;
  int16_t                 dataType;
  std::string             name;
  std::vector<Geometry *> geometries;
};

struct Library {
  Units                   units;
  Coord                   min,
                          max;
  std::string             name;
  std::vector<Element *>  elements;
  std::vector<Layer>      layers;
public:
  Library() : units({ 0.001 , 1e-9}), min({ -1, -1 }), max({ -1, -1 }) {
  }
 ~Library() {
    for (size_t i = 0; i < elements.size(); ++i) {
      delete elements[i];
      elements[i] = nullptr;
    }
    elements.clear();
    layers.clear();
  }
};

enum class FileFormat {
  undefined,
  CIF,
  DXF,
  GDSII_ascii,
  GDSII_bin,
  MAG,
  MSK,
  OASIS,
};

struct Layout {
  std::wstring            fileName;
  FileFormat              fileFormat;
  std::vector<Library *>  libraries;
public:
  ~Layout() {
    for (size_t i = 0; i < libraries.size(); ++i) {
      delete libraries[i];
      libraries[i] = nullptr;
    }
    libraries.clear();
  }
};

//!!! Don't touch these lines on comparison

struct LayoutReaderOptions {
  std::vector<int>          layersToRead;
  std::vector<int>          layersToIgnore;
  std::vector<std::string>  cellsToRead;
  bool                      convertPolysToRects,
                            ignoreZeroWidthPaths;
};

typedef Layout *(*Func_CreateLayout)  (std::wstring);
typedef void    (*Func_ReadLayout)    (std::wstring, Layout *, LayoutReaderOptions *);
typedef void    (*Func_ClearLayout)   (Layout *);
typedef void    (*Func_CopyLayout)    (Layout *, Layout *);
typedef bool    (*Func_WriteLayout)   (Layout *, std::wstring, FileFormat);
typedef void    (*Func_FreeLayout)    (Layout **);

__declspec(dllexport) Layout *CreateLayoutFromFile(std::wstring fileName, LayoutReaderOptions *options);
__declspec(dllexport) void    ReadLayoutFromFile(std::wstring fileName, Layout *layout, LayoutReaderOptions *options);
__declspec(dllexport) void    ClearLayout(Layout *layout);
__declspec(dllexport) void    CopyLayout(Layout *src, Layout *dst);
__declspec(dllexport) bool    WriteLayout(Layout *layout, std::wstring fileName, FileFormat format);
__declspec(dllexport) void    FreeLayout(Layout **layout);
