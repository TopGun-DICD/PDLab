#include "FlowItem_OR.hpp"

#include <ctime>

#include "../Logger.hpp"
#include "FlowItemPort.hpp"
#include "FlowItemConnection.hpp"
#include "LayoutLoader.hpp"
#include "../Helper.hpp"
#include "../Config.hpp"

FlowItem_OR::FlowItem_OR(BasicLogger *logger) : FlowItem(FlowItemType::OR, QString("OR"), logger, LayoutOwnershipMode::make_new) {
  AddInputPort(PortDataType::layout);
  AddInputPort(PortDataType::layout);
  AddOutputPort(PortDataType::layout);
  titleBgColor = Config::Get()->colors.headerLayoutOperations;
  //bottomString = QString("KLayout");
}

FlowItem_OR::~FlowItem_OR() {
}

bool FlowItem_OR::DropEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'OR-DROP' was called");
#endif

  return true;
}

void CopyAndFlattenLayoutElement(Element *dst, Element *src) {
  dst->min = src->min;
  dst->max = src->max;
  //TODO: j -> i
  for (size_t j = 0; j < src->geometries.size(); ++j) {

    Polygon   *p_polygon  = nullptr;
    Path      *p_path     = nullptr;
    Text      *p_text     = nullptr;
    Rectangle *p_rect     = nullptr;
    Reference *p_ref      = nullptr;

    switch (src->geometries[j]->type) {
    case GeometryType::polygon:
      p_polygon = new Polygon;
      dst->geometries.push_back(p_polygon);
      p_polygon->type = GeometryType::polygon;
      p_polygon->layer = 1;
      for (size_t l = 0; l < src->geometries[j]->properties.size(); ++l)
        //TODO: Faster copying? p_polygon->properties = src->geometries[j]->properties???
        p_polygon->properties.push_back(src->geometries[j]->properties[l]);
      for (size_t l = 0; l < src->geometries[j]->coords.size(); ++l)
        p_polygon->coords.push_back(src->geometries[j]->coords[l]);
      p_polygon->dataType = static_cast<Polygon *>(src->geometries[j])->dataType;
      break;
    case GeometryType::rectangle:
      p_rect = new Rectangle;
      dst->geometries.push_back(p_rect);
      p_rect->type = GeometryType::rectangle;
      p_rect->layer = 1;
      for (size_t l = 0; l < src->geometries[j]->properties.size(); ++l)
        p_rect->properties.push_back(src->geometries[j]->properties[l]);
      for (size_t l = 0; l < src->geometries[j]->coords.size(); ++l)
        p_rect->coords.push_back(src->geometries[j]->coords[l]);
      p_rect->rectType = static_cast<Rectangle *>(src->geometries[j])->rectType;
      break;
    case GeometryType::path:
      p_path = new Path;
      dst->geometries.push_back(p_path);
      p_path->type = GeometryType::path;
      p_path->layer = 1;
      for (size_t l = 0; l < src->geometries[j]->properties.size(); ++l)
        p_path->properties.push_back(src->geometries[j]->properties[l]);
      for (size_t l = 0; l < src->geometries[j]->coords.size(); ++l)
        p_path->coords.push_back(src->geometries[j]->coords[l]);
      p_path->dataType = static_cast<Path *>(src->geometries[j])->dataType;
      p_path->pathType = static_cast<Path *>(src->geometries[j])->pathType;
      p_path->width = static_cast<Path *>(src->geometries[j])->width;
      break;
    case GeometryType::text:
      p_text = new Text;
      dst->geometries.push_back(p_text);
      p_text->type = GeometryType::text;
      p_text->layer = 1;
      for (size_t l = 0; l < src->geometries[j]->properties.size(); ++l)
        p_text->properties.push_back(src->geometries[j]->properties[l]);
      for (size_t l = 0; l < src->geometries[j]->coords.size(); ++l)
        p_text->coords.push_back(src->geometries[j]->coords[l]);
      p_text->textType = static_cast<Text *>(src->geometries[j])->textType;
      p_text->pathType = static_cast<Text *>(src->geometries[j])->pathType;
      p_text->width = static_cast<Text *>(src->geometries[j])->width;
      p_text->flagsPresentation = static_cast<Text *>(src->geometries[j])->flagsPresentation;
      p_text->flagsTransformation = static_cast<Text *>(src->geometries[j])->flagsTransformation;
      p_text->magnification = static_cast<Text *>(src->geometries[j])->magnification;
      p_text->stringValue = static_cast<Text *>(src->geometries[j])->stringValue;
      break;
    case GeometryType::reference:
      CopyAndFlattenLayoutElement(dst, static_cast<Reference *>(src->geometries[j])->referenceTo);
      break;
    }
  }
}

bool FlowItem_OR::ExecuteEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'OR-EXECUTE' was called");
#endif

  if (inputPorts[0]->connections.empty() || inputPorts[1]->connections.empty()) {
    p_logger->Error("Can't perform 'OR' operation. One or more ports are not connected.");
    return false;
  }

  Layout *p_layout1 = inputPorts[0]->GetLayout();
  Layout *p_layout2 = inputPorts[1]->GetLayout();

  if (!p_layout1 || !p_layout2) {
    p_logger->Error("Can't perform 'OR' operation. At least one of the input layouts is NULL.");
    return false;
  }
  double deltaPhysical = p_layout1->libraries[0]->units.physical - p_layout2->libraries[0]->units.physical;
  double deltaUser = p_layout1->libraries[0]->units.user - p_layout2->libraries[0]->units.user;
  if (fabs(deltaPhysical) > 1e-12 || fabs(deltaUser) > 1e-12) {
    p_logger->Error("Can't perform 'OR' operation. Units in both layout are not identical. Don't know what to do :(");

    return false;
  }

  p_resultLayout->libraries.push_back(new Library);
  p_resultLayout->libraries[0]->name = "PDLAB.OR";
  p_resultLayout->libraries[0]->units = p_layout1->libraries[0]->units;

  Element *p_element = new Element;

  p_resultLayout->libraries[0]->elements.push_back(p_element);
  p_element->name = "OR_RESULT";
  p_element->nested = false;

  for (size_t i = 0; i < p_layout1->libraries[0]->elements.size(); ++i)
    CopyAndFlattenLayoutElement(p_element, p_layout1->libraries[0]->elements[i]);
  for (size_t i = 0; i < p_layout2->libraries[0]->elements.size(); ++i)
    CopyAndFlattenLayoutElement(p_element, p_layout2->libraries[0]->elements[i]);

  Layer l;
  l.layer = 1;
  p_resultLayout->libraries[0]->layers.push_back(l);
  
  //TODO: j -> i
  for (size_t j = 0; j < p_resultLayout->libraries[0]->elements.size(); ++j) {
    p_resultLayout->libraries[0]->min = p_resultLayout->libraries[0]->elements[j]->min;
    p_resultLayout->libraries[0]->max = p_resultLayout->libraries[0]->elements[j]->max;

    if (p_resultLayout->libraries[0]->min.x > p_resultLayout->libraries[0]->elements[j]->min.x)
      p_resultLayout->libraries[0]->min.x = p_resultLayout->libraries[0]->elements[j]->min.x;
    if (p_resultLayout->libraries[0]->min.y > p_resultLayout->libraries[0]->elements[j]->min.y)
      p_resultLayout->libraries[0]->min.y = p_resultLayout->libraries[0]->elements[j]->min.y;
    if (p_resultLayout->libraries[0]->max.x > p_resultLayout->libraries[0]->elements[j]->max.x)
      p_resultLayout->libraries[0]->max.x = p_resultLayout->libraries[0]->elements[j]->max.x;
    if (p_resultLayout->libraries[0]->max.y > p_resultLayout->libraries[0]->elements[j]->max.y)
      p_resultLayout->libraries[0]->max.y = p_resultLayout->libraries[0]->elements[j]->max.y;
  }


  // Maybe this will be useful for merge?
  /*for (size_t i = 0; i < p_resultLayout->libraries[0]->elements.size(); ++i) {
    for (size_t j = 0; j < p_resultLayout->libraries[0]->elements[i]->geometries.size(); ++j) {
      if (p_resultLayout->libraries[0]->elements[i]->geometries[j]->type == GeometryType::reference)
        continue;
      p_resultLayout->libraries[0]->elements[i]->geometries[j]->layer = 1;
    }
  }

  Polygon   *p_polygon = nullptr;
  Path      *p_path    = nullptr;
  Text      *p_text    = nullptr;
  Box       *p_box     = nullptr;
  Reference *p_ref     = nullptr;
  
  for (size_t i = 0; i < inputPorts[1]->GetLayout()->libraries[0]->elements.size(); ++i) {
    Element *p_element = new Element;
    p_element->name = inputPorts[1]->GetLayout()->libraries[0]->elements[i]->name;
    p_element->nested = inputPorts[1]->GetLayout()->libraries[0]->elements[i]->nested;
    p_element->min = inputPorts[1]->GetLayout()->libraries[0]->elements[i]->min;
    p_element->max = inputPorts[1]->GetLayout()->libraries[0]->elements[i]->max;
    p_resultLayout->libraries[0]->elements.push_back(p_element);
    for (size_t j = 0; j < inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries.size(); ++j) {
      switch (inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j]->type) {
        case GeometryType::polygon:
          p_polygon = new Polygon;
          p_element->geometries.push_back(p_polygon);
          p_polygon->type = GeometryType::polygon;
          p_polygon->layer = 1;
          for (size_t l = 0; l < inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j]->properties.size(); ++l)
            p_polygon->properties.push_back(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j]->properties[l]);
          for (size_t l = 0; l < inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j]->coords.size(); ++l)
            p_polygon->coords.push_back(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j]->coords[l]);
          p_polygon->dataType = static_cast<Polygon *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j])->dataType;
          break;
        case GeometryType::box:
          p_box = new Box;
          p_element->geometries.push_back(p_box);
          p_box->type = GeometryType::box;
          p_box->layer = 1;
          for (size_t l = 0; l < inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j]->properties.size(); ++l)
            p_box->properties.push_back(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j]->properties[l]);
          for (size_t l = 0; l < inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j]->coords.size(); ++l)
            p_box->coords.push_back(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j]->coords[l]);
          p_box->boxType = static_cast<Box *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j])->boxType;
          break;
        case GeometryType::path:
          p_path = new Path;
          p_element->geometries.push_back(p_path);
          p_path->type = GeometryType::path;
          p_path->layer = 1;
          for (size_t l = 0; l < inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j]->properties.size(); ++l)
            p_path->properties.push_back(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j]->properties[l]);
          for (size_t l = 0; l < inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j]->coords.size(); ++l)
            p_path->coords.push_back(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j]->coords[l]);
          p_path->dataType = static_cast<Path *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j])->dataType;
          p_path->pathType = static_cast<Path *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j])->pathType;
          p_path->width = static_cast<Path *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j])->width;
          break;
        case GeometryType::text:
          p_text = new Text;
          p_element->geometries.push_back(p_text);
          p_text->type = GeometryType::text;
          p_text->layer = 1;
          for (size_t l = 0; l < inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j]->properties.size(); ++l)
            p_text->properties.push_back(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j]->properties[l]);
          for (size_t l = 0; l < inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j]->coords.size(); ++l)
            p_text->coords.push_back(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j]->coords[l]);
          p_text->textType = static_cast<Text *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j])->textType;
          p_text->pathType = static_cast<Text *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j])->pathType;
          p_text->width = static_cast<Text *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j])->width;
          p_text->flagsPresentation = static_cast<Text *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j])->flagsPresentation;
          p_text->flagsTransformation = static_cast<Text *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j])->flagsTransformation;
          p_text->magnification = static_cast<Text *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j])->magnification;
          p_text->stringValue = static_cast<Text *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j])->stringValue;
          break;
        case GeometryType::reference:
          p_ref = new Reference;
          p_element->geometries.push_back(p_ref);
          p_ref->type = GeometryType::reference;
          p_ref->layer = 1;
          for (size_t l = 0; l < inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j]->properties.size(); ++l)
            p_ref->properties.push_back(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j]->properties[l]);
          for (size_t l = 0; l < inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j]->coords.size(); ++l)
            p_ref->coords.push_back(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j]->coords[l]);
          p_ref->name = static_cast<Reference *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j])->name;
          p_ref->referenceTo = static_cast<Reference *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j])->referenceTo;
          p_ref->transformationFlags = static_cast<Reference *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j])->transformationFlags;
          p_ref->magnification = static_cast<Reference *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->geometries[j])->magnification;
          break;
      }

    } // !

  }

  p_resultLayout->libraries[0]->layers.clear();
  Layer layer;
  layer.layer = 1;
  layer.name = "OR_layer";
  p_resultLayout->libraries[0]->layers.push_back(layer);

  for (size_t i = 0; i < p_resultLayout->libraries[0]->elements.size(); ++i) {
    for (size_t j = 0; j < p_resultLayout->libraries[0]->elements[i]->geometries.size(); ++j) {
      if (p_resultLayout->libraries[0]->elements[i]->geometries[j]->type == GeometryType::reference)
        continue;
      p_resultLayout->libraries[0]->layers[0].items.push_back(p_resultLayout->libraries[0]->elements[i]->geometries[j]);
    }
  }*/

  return true;
}

bool FlowItem_OR::OpenResultsEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'OR-OPENRESULTS' was called");
#endif

  Helper::GetInstance()->ShowLayout(p_resultLayout);

  return true;
}

bool FlowItem_OR::ResetEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'OR-RESET' was called");
#endif

  return true;
}

bool FlowItem_OR::ShowPropertesEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'OR-PROPERTIES' was called");
#endif

  return true;
}

QString FlowItem_OR::GetInfoString() {
  return QString("OR info string");
}