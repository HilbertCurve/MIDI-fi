#include "ui/UIElement.h"

#include <string.h>
#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>

#include "graphics/rData.h"
#include "utils/Utils.h"

namespace MidiFi
{
    namespace UI
    {
        void UIElement::init(glm::vec3 pos, glm::vec4 color, float width, float height)
        {
            this->pos = pos;
            this->color = color;
            this->width = width;
            this->height = height;
        }

        using namespace Graphics; // fight me
        int UIElement::toRData(rData &r, unsigned int rOffset)
        {
            __pAssert(!(rOffset >= r.vertCount / 4), "rData not big enough to hold game states!");

            int stride = rOffset * getLayoutLen(r) * 4;
            for (int i = 0; i < 4; i++)
            {
                glm::vec3 orientation;
                switch (i)
                {
                    case 0: orientation = {1.0f * this->width, 1.0f * this->height, 0.0f}; break;
                    case 1: orientation = {0.0f * this->width, 1.0f * this->height, 0.0f}; break;
                    case 2: orientation = {0.0f * this->width, 0.0f * this->height, 0.0f}; break;
                    case 3: orientation = {1.0f * this->width, 0.0f * this->height, 0.0f}; break;
                }

                off_len result = getAttribMetaData(r, PONT_POS);
                if (result.second >= 3 * sizeof(float))
                {
                    this->pos += orientation - glm::vec3{this->width / 2, this->height / 2, 0.0f};

                    // TODO: just use memcpy, bonehead.
                    for (int j = 0; j < 3; j++)
                    {
                        ((float *)((char *)r.data + result.first + stride))[j] = ((float *)&this->pos)[j];
                    }

                    this->pos -= orientation - glm::vec3{this->width / 2, this->height / 2, 0.0f};
                }
                
                result = getAttribMetaData(r, PONT_COLOR);
                if (result.second >= 4 * sizeof(float))
                {
                    for (int j = 0; j < 4; j++)
                    {
                        ((float *)((char *)r.data + result.first + stride))[j] = ((float *)&this->color)[j];
                    }             
                }

                result = getAttribMetaData(r, PONT_TEXCOORD);
                
                if (result.second >= 2 * sizeof(float))
                {
                    orientation.x /= this->width;
                    orientation.y /= this->height;
                    for (int j = 0; j < 2; j++)
                    {
                        ((float *)((char *)r.data + result.first + stride))[j] = this->tex.texCoords[j + i * 2];
                    }
                }

                result = getAttribMetaData(r, PONT_TEXID);
                if (result.second == 1 * sizeof(float)) // I'd be very confused if there was more than one texID.
                {
                    if (this->tex.source == nullptr)
                    {
                        *(float *)((char *)r.data + result.first + stride) = 0.0f;
                    }
                    else
                    {
                        *(float *)((char *)r.data + result.first + stride) = 6.0f;//this->tex.source->texID;
                    }
                }
                stride += getLayoutLen(r);
            }

            r.isDirty = true;
            
            return stride / (4 * getLayoutLen(r));
        }

        void UIElement::toRData(rData &r, unsigned int rOffset, vProp property)
        {
            int offset = rOffset * 4 * getLayoutLen(r);
            
            off_len result = getAttribMetaData(r, property);
            for (int i = 0; i < 4; i++)
            {

                glm::vec3 orientation;
                switch (i)
                {
                    case 0: orientation = {1.0f * this->width, 1.0f * this->height, 0.0f}; break;
                    case 1: orientation = {0.0f * this->width, 1.0f * this->height, 0.0f}; break;
                    case 2: orientation = {0.0f * this->width, 0.0f * this->height, 0.0f}; break;
                    case 3: orientation = {1.0f * this->width, 0.0f * this->height, 0.0f}; break;
                }

                switch (property)
                {
                    case PONT_POS:
                    {
                        if (result.second >= 3 * sizeof(float))
                        {
                            this->pos += orientation - glm::vec3{this->width / 2, this->height / 2, 0.0f};

                            for (int j = 0; j < 3; j++)
                            {
                                ((float *)((char *)r.data + result.first + offset))[j] = ((float *)&this->pos)[j];
                            }

                            this->pos -= orientation - glm::vec3{this->width / 2, this->height / 2, 0.0f};
                        }
                    } break;
                    case PONT_COLOR:
                    {
                        if (result.second >= 4 * sizeof(float))
                        {
                            for (int j = 0; j < 4; j++)
                            {
                                ((float *)((char *)r.data + result.first + offset))[j] = ((float *)&this->color)[j];
                            }
                        }
                    } break;
                    case PONT_TEXCOORD:
                    {
                        if (result.second >= 2 * sizeof(float))
                        {
                            orientation.x /= this->width;
                            orientation.y /= this->height;
                            for (int j = 0; j < 2; j++)
                            {
                                *(float *)((char *)r.data + result.first + offset) = this->tex.texCoords[j + i * 2];
                            }
                        }
                    } break;
                    case PONT_TEXID:
                    {
                        __pMessage("Don't change the TexID!"); // at least not yet
                    }
                }

                offset += getLayoutLen(r);
            }

            r.isDirty = true;
        }
    }
}