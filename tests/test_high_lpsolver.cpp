//
// Created by dewliak on 4/2/26.
//

#include <Highs.h>
#include <gtest/gtest.h>


TEST(LPSOLVE, INIT) {
    HighsModel model;
    model.lp_.num_col_ = 2;
    model.lp_.num_row_ = 3;
    model.lp_.sense_ = ObjSense::kMinimize;
    model.lp_.offset_ = 3;
    model.lp_.col_cost_ = {1.0, 1.0};
    model.lp_.col_lower_ = {0.0, 1.0};
    model.lp_.col_upper_ = {4.0, 1.0e30};
    model.lp_.row_lower_ = {-1.0e30, 5.0, 6.0};
    model.lp_.row_upper_ = {7.0, 15.0, 1.0e30};

    // Here the orientation of the matrix is column-wise
    model.lp_.a_matrix_.format_ = MatrixFormat::kColwise;
    // a_start_ has num_col_+1 entries, and the last entry is the number
    // of nonzeros in A, allowing the number of nonzeros in the last
    // column to be defined
    model.lp_.a_matrix_.start_ = {0, 2, 5};
    model.lp_.a_matrix_.index_ = {1, 2, 0, 1, 2};
    model.lp_.a_matrix_.value_ = {1.0, 3.0, 1.0, 2.0, 2.0};


    // Create a Highs instance
    Highs highs;
    HighsStatus return_status;

    // Pass the model to HiGHS
    return_status = highs.passModel(model);
    assert(return_status==HighsStatus::kOk);

    // Get a const reference to the LP data in HiGHS
    const HighsLp& lp = highs.getLp();

    // Solve the model
    return_status = highs.run();

    // Get the model status
    const HighsModelStatus& model_status = highs.getModelStatus();


    EXPECT_EQ(return_status,HighsStatus::kOk);
    EXPECT_EQ(model_status,HighsModelStatus::kOptimal);
}