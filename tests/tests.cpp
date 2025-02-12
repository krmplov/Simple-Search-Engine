#include <Index/Indexer.h>
#include "FilesWork/FileWorker.h"
#include <Search/Search.h>
#include <Search/QueryParserSearch.h>
#include <Search/BM25.h>
#include <Search/TF_IDF.h>

#include <gtest/gtest.h>

class BM25Test : public ::testing::Test {
protected:
    BM25 bm25;
};

TEST_F(BM25Test, TestScore) {
    bm25.SetDlAvg(100);
    double score = bm25.Score(10, 50);
    ASSERT_NEAR(score, 2.14, 0.01);
}

TEST_F(BM25Test, TestAssignmentOperator) {
    BM25 bm25_other;
    bm25_other.SetDlAvg(200);
    bm25 = std::move(bm25_other);
    double score = bm25.Score(10, 50);
    ASSERT_NEAR(score, 2.14, 0.01);
}

TEST_F(BM25Test, TestSetDlAvg) {
    bm25.SetDlAvg(300);
    double score = bm25.Score(10, 50);
    ASSERT_NEAR(score, 2.14, 0.01);
}

TEST_F(BM25Test, TestSetDlAvg2) {
    bm25.SetDlAvg(100);
    double score = bm25.Score(6, 300);
    ASSERT_NEAR(score, 1.47, 0.01);
}

TEST_F(BM25Test, TestSetDlAvg3) {
    bm25.SetDlAvg(20);
    double score = bm25.Score(7, 30);
    ASSERT_NEAR(score, 1.88, 0.01);
}

TEST_F(BM25Test, TestSetDlAvg4) {
    bm25.SetDlAvg(100);
    double score = bm25.Score(5, 11);
    ASSERT_NEAR(score, 2.07, 0.01);
}

TEST_F(BM25Test, TestSetDlAvg5) {
    bm25.SetDlAvg(55);
    double score = bm25.Score(30, 30);
    ASSERT_NEAR(score, 2.18, 0.01);
}

class TF_IDFTest : public ::testing::Test {
protected:
    TF_IDF tf_idf;
};

TEST_F(TF_IDFTest, TestScore) {
    tf_idf = TF_IDF(100, 1000);
    double score = tf_idf.Score(10, 50, 100);
    ASSERT_NEAR(score, 4.92, 0.01);
}

TEST_F(TF_IDFTest, TestAssignmentOperator) {
    TF_IDF tf_idf_other(200, 2000);
    tf_idf = std::move(tf_idf_other);
    double score = tf_idf.Score(10, 50, 100);
    ASSERT_NEAR(score, 6.4, 0.01);
}

TEST_F(TF_IDFTest, TestScore2) {
    tf_idf = TF_IDF(500, 600);
    double score = tf_idf.Score(50, 50, 250);
    ASSERT_NEAR(score, 1.91, 0.01);
}

TEST_F(TF_IDFTest, TestScore3) {
    tf_idf = TF_IDF(1, 1);
    double score = tf_idf.Score(1, 5, 1);
    ASSERT_NEAR(score, 0, 0.01);
}

TEST_F(TF_IDFTest, TestScore4) {
    tf_idf = TF_IDF(40000, 1000);
    double score = tf_idf.Score(55, 60, 100);
    ASSERT_NEAR(score, 5.04, 0.01);
}

TEST_F(TF_IDFTest, TestScore5) {
    tf_idf = TF_IDF(222, 333);
    double score = tf_idf.Score(2, 50, 100);
    ASSERT_NEAR(score, 2.3, 0.01);
}

TEST_F(TF_IDFTest, TestScore6) {
    tf_idf = TF_IDF(67, 113);
    double score = tf_idf.Score(21, 72, 90);
    ASSERT_NEAR(score, 0.47, 0.01);
}

TEST_F(TF_IDFTest, TestScore7) {
    tf_idf = TF_IDF(378, 456);
    double score = tf_idf.Score(78, 113, 200);
    ASSERT_NEAR(score, 1.8, 0.01);
}

TEST_F(TF_IDFTest, TestScore8) {
    tf_idf = TF_IDF(12, 22);
    double score = tf_idf.Score(7, 11, 20);
    ASSERT_NEAR(score, 0.2, 0.01);
}

class ParserSearchTest : public ::testing::Test {
protected:
    ParserSearch parser;
    std::set<std::string> posting_lists;
};

TEST_F(ParserSearchTest, TestGetTree) {
    std::string request = "(apple AND orange) OR (banana AND grape)";
    std::unique_ptr<Node> tree = ParserSearch::GetTree(request, posting_lists);
    ASSERT_EQ(tree->element.type, Type::OR);
    ASSERT_EQ(tree->left->element.type, Type::AND);
    ASSERT_EQ(tree->right->element.type, Type::AND);
    ASSERT_EQ(tree->left->left->element.value, "apple");
    ASSERT_EQ(tree->left->right->element.value, "orange");
    ASSERT_EQ(tree->right->left->element.value, "banana");
    ASSERT_EQ(tree->right->right->element.value, "grape");
}

TEST_F(ParserSearchTest, TestGetTree2) {
    std::string request = "for";
    std::unique_ptr<Node> tree = ParserSearch::GetTree(request, posting_lists);
    ASSERT_EQ(tree->element.type, Type::WORD);
    ASSERT_EQ(tree->element.value, "for");
}

TEST_F(ParserSearchTest, TestGetTree3) {
    std::string request = "vector OR list";
    std::unique_ptr<Node> tree = ParserSearch::GetTree(request, posting_lists);
    ASSERT_EQ(tree->element.type, Type::OR);
    ASSERT_EQ(tree->left->element.value, "vector");
    ASSERT_EQ(tree->right->element.value, "list");
}


TEST_F(ParserSearchTest, TestGetTree4) {
    std::string request = "vector AND list";
    std::unique_ptr<Node> tree = ParserSearch::GetTree(request, posting_lists);
    ASSERT_EQ(tree->element.type, Type::AND);
    ASSERT_EQ(tree->left->element.value, "vector");
    ASSERT_EQ(tree->right->element.value, "list");
}

TEST_F(ParserSearchTest, TestGetTree5) {
    std::string request = "(for)";
    std::unique_ptr<Node> tree = ParserSearch::GetTree(request, posting_lists);
    ASSERT_EQ(tree->element.type, Type::WORD);
    ASSERT_EQ(tree->element.value, "for");
}

TEST_F(ParserSearchTest, TestGetTree6) {
    std::string request = "(vector OR list)";
    std::unique_ptr<Node> tree = ParserSearch::GetTree(request, posting_lists);
    ASSERT_EQ(tree->element.type, Type::OR);
    ASSERT_EQ(tree->left->element.value, "vector");
    ASSERT_EQ(tree->right->element.value, "list");
}

TEST_F(ParserSearchTest, TestGetTree7) {
    std::string request = "(vector AND list)";
    std::unique_ptr<Node> tree = ParserSearch::GetTree(request, posting_lists);
    ASSERT_EQ(tree->element.type, Type::AND);
    ASSERT_EQ(tree->left->element.value, "vector");
    ASSERT_EQ(tree->right->element.value, "list");
}

TEST_F(ParserSearchTest, TestGetTree8) {
    std::string request = "(while OR for) AND vector";
    std::unique_ptr<Node> tree = ParserSearch::GetTree(request, posting_lists);
    ASSERT_EQ(tree->element.type, Type::AND);
    ASSERT_EQ(tree->left->element.type, Type::OR);
    ASSERT_EQ(tree->right->element.type, Type::WORD);
    ASSERT_EQ(tree->right->element.value, "vector");
    ASSERT_EQ(tree->left->left->element.value, "while");
    ASSERT_EQ(tree->left->right->element.value, "for");
}

TEST_F(ParserSearchTest, TestGetTree9) {
    std::string request = "for AND and";
    std::unique_ptr<Node> tree = ParserSearch::GetTree(request, posting_lists);
    ASSERT_EQ(tree->element.type, Type::AND);
    ASSERT_EQ(tree->left->element.value, "for");
    ASSERT_EQ(tree->right->element.value, "and");
}

TEST_F(ParserSearchTest, TestInvalidQuery) {
    std::string request = "apple AND OR orange";
    ASSERT_THROW(ParserSearch::GetTree(request, posting_lists), std::runtime_error);
}

TEST_F(ParserSearchTest, TestInvalidQuery2) {
    std::string request = "for AND";
    ASSERT_THROW(ParserSearch::GetTree(request, posting_lists), std::runtime_error);
}

TEST_F(ParserSearchTest, TestInvalidQuery3) {
    std::string request = "vector list";
    ASSERT_THROW(ParserSearch::GetTree(request, posting_lists), std::runtime_error);
}

TEST_F(ParserSearchTest, TestInvalidQuery4) {
    std::string request = "vector Or list";
    ASSERT_THROW(ParserSearch::GetTree(request, posting_lists), std::runtime_error);
}


