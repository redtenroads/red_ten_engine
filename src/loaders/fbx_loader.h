#pragma once
#include <string>
#include "common/meshDescriptor.h"

struct NodeDataBinding{
    char type;
    void *data;
    unsigned int numElements;
};

class Node
{
public:
    Node(int level, FILE *file);

    void process();
    void readHead();
    void readValues();
    void print();
    bool isName(const char *name);
    Node *findNode(const char *name);
    bool doSkip(const char *name);

    std::vector<Node *> children;

    unsigned int endOffset = 0;
    unsigned int numProperties = 0;
    unsigned int propertyListLen = 0;
    char *name = nullptr;
    bool bIsZero = false;
    bool bMarkedToRemove = false;
    int level = 0;
    FILE *file = nullptr;
    std::vector<NodeDataBinding> bindedData;
};

struct NodeRecordHeader
{
    unsigned int endOffset;
    unsigned int numProperties;
    unsigned int propertyListLen;
};

class FBXLoader
{
public:
    bool load(std::string meshPath);
    void print();

    MeshDescriptor *root = nullptr;
    MeshDescriptor *current = nullptr;

    FILE *file = nullptr;

protected:
    void sendToMeshDescriptor(MeshDescriptor *meshDescriptor);

    int readPosition = 0;

    double *dData = nullptr;
    float *fData = nullptr;
    int *iData = nullptr;

    std::vector<Node *> nodes;
    NodeRecordHeader header;

    int dataLength = 0;
};
