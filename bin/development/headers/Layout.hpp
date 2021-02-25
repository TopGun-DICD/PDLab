#pragma once

#include <string>
#include <vector>

#pragma warning(disable: 26495) // Warning C26495 Variable 'GeometryItem::layer' is uninitialized. Always initialize a member variable(type.6).

// Used in GDSII
struct Units {
  double  user,
          physical;
};

struct Coord {
  __int32 x,
          y;
};

// Used in GDSII
struct Property {
  __int16     index;
  std::string value;
};

enum class GeometryType {
  undefined = 0,
  polygon,
  path,
  text,
  box,
  reference,
};

enum class FileFormat {
  undefined = 0,
  GDSII_bin,
  GDSII_ASCII,
  MSK,
  OASIS,
};

struct Geometry {
  GeometryType            type;
  __int16                 layer;
  std::vector<Property>   properties;
  std::vector<Coord>      coords;
};

struct Geometry_Polygon : public Geometry {
  __int16                 dataType;
};

struct Geometry_Path : public Geometry {
  __int16                 dataType,
                          pathType;
  __int32                 width;
};

struct Geometry_Text : public Geometry {
  __int16                 textType,
                          flagsPresentation,
                          pathType,
                          flagsTransformation;
  double                  magnification;
  __int32                 width;
  std::string             stringValue;
};

struct Geometry_Box : public Geometry {
  __int16                 boxType;
};

struct Element;

struct Geometry_Reference : public Geometry {
  std::string             name;
  Element                *referenceTo;
  __int16                 transformationFlags;
  double                  magnification;
};

struct Element {
  std::string             name;
  std::vector<Geometry *> items;
  Coord                   min,
                          max;
  bool                    nested;
};

struct Layer {
  __int16                 layer;
  std::string             name;
  std::vector<Geometry *> items;
};

struct Library {
  std::string             name;
  Units                   units;
  std::vector<Element *>  elements;
  std::vector<Layer>      layers;
  Coord                   min,
                          max;
};

struct Layout {
  std::wstring            fileName;
  FileFormat              fileFormat;
  std::vector<Library *>  libraries;
};

typedef Layout *(*Func_CreateLayout)  (std::wstring);
typedef void    (*Func_ReadLayout)    (std::wstring, Layout *);
typedef void    (*Func_ClearLayout)   (Layout *);
typedef void    (*Func_CopyLayout)    (Layout *src, Layout *dst);
typedef void    (*Func_FreeLayout)    (Layout **);

__declspec(dllexport) Layout *CreateLayoutFromFile(std::wstring fileName);
__declspec(dllexport) void    ReadLayoutFromFile(std::wstring fileName, Layout *layout);
__declspec(dllexport) void    ClearLayout(Layout *layout);
__declspec(dllexport) void    CopyLayout(Layout *src, Layout *dst);
__declspec(dllexport) void    FreeLayout(Layout **layout);
