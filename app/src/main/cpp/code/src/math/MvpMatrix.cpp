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
            : mModel{}, mView{}, mProjection{} {}

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

} // matrix