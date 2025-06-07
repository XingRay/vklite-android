//
// Created by leixing on 2025/6/7.
//

#include "MvpMatrix.h"

namespace math {

    MvpMatrix::MvpMatrix(glm::mat4 model,
                         glm::mat4 view,
                         glm::mat4 projection)
            : mModel(model), mView(view), mProjection(projection) {}

    MvpMatrix::MvpMatrix()
            : mModel{glm::mat4(1.0f)}, mView{glm::mat4(1.0f)}, mProjection{glm::mat4(1.0f)} {}

    MvpMatrix::~MvpMatrix() = default;

    MvpMatrix &MvpMatrix::model(glm::mat4 model) {
        mModel = model;
        return *this;
    }

    MvpMatrix &MvpMatrix::view(glm::mat4 view) {
        mView = view;
        return *this;
    }

    MvpMatrix &MvpMatrix::projection(glm::mat4 projection) {
        mProjection = projection;
        return *this;
    }

    glm::mat4 &MvpMatrix::getModel() {
        return mModel;
    }

    glm::mat4 &MvpMatrix::getView() {
        return mView;
    }

    glm::mat4 &MvpMatrix::getProjection() {
        return mProjection;
    }

    glm::mat4 MvpMatrix::calcMvp() {
        return mProjection * mView * mModel;
    }

    MvpMatrix &MvpMatrix::modelRotateX(float angle) {
        mModel = glm::rotate(mModel, angle, glm::vec3(1.0f, 0.0f, 0.0f));
        return *this;
    }

    MvpMatrix &MvpMatrix::modelRotateY(float angle) {
        mModel = glm::rotate(mModel, angle, glm::vec3(0.0f, 1.0f, 0.0f));
        return *this;
    }

    MvpMatrix &MvpMatrix::modelRotateZ(float angle) {
        mModel = glm::rotate(mModel, angle, glm::vec3(0.0f, 0.0f, 1.0f));
        return *this;
    }

    MvpMatrix &MvpMatrix::modelScale(float scaleX, float scaleY, float scaleZ) {
        mModel = glm::scale(mModel, glm::vec3(scaleX, scaleY, scaleZ));
        return *this;
    }

    MvpMatrix &MvpMatrix::modelScaleX(float scale) {
        modelScale(scale, 1.0f, 1.0f);
        return *this;
    }

    MvpMatrix &MvpMatrix::modelScaleY(float scale) {
        modelScale(1.0f, scale, 1.0f);
        return *this;
    }

    MvpMatrix &MvpMatrix::modelScaleZ(float scale) {
        modelScale(1.0f, 1.0f, scale);
        return *this;
    }

} // matrix