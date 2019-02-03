/*******************************************************************************
 * Copyright (c) 2015-2018 Skymind, Inc.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Apache License, Version 2.0 which is available at
 * https://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 ******************************************************************************/

//
// @author raver119@gmail.com
//

#include "testlayers.h"
#include <ops/declarable/CustomOperations.h>
#include <NDArray.h>
#include <ops/ops.h>
#include <GradCheck.h>


using namespace nd4j;


class NlpTests : public testing::Test {
public:

    NlpTests() {
        printf("\n");
        fflush(stdout);
    }
};

TEST_F(NlpTests, basic_sg_hs_test_1) {
    auto exp0 = NDArrayFactory::create<float>('c', {1, 10});
    auto exp1 = NDArrayFactory::create<float>('c', {1, 10});

    exp0.assign(0.01001f);
    exp1.assign(0.020005f);

    auto target = NDArrayFactory::create<int>(0);
    auto ngStarter = NDArrayFactory::empty<int>();
    auto indices = NDArrayFactory::create<int>('c', {1}, {1});
    auto codes = NDArrayFactory::create<int8_t>('c', {1});
    auto syn0 = NDArrayFactory::create<float>('c', {100, 10});
    auto syn1 = NDArrayFactory::create<float>('c', {100, 10});
    auto syn1Neg = NDArrayFactory::empty<float>();
    auto expTable = NDArrayFactory::create<float>('c', {10000});
    auto negTable = NDArrayFactory::empty<float>();

    syn0.assign(0.01);
    syn1.assign(0.02);
    expTable.assign(0.5);

    auto alpha = NDArrayFactory::create<double>(0.001);
    auto randomValue = NDArrayFactory::create<Nd4jLong>(1L);
    auto inferenceVector = NDArrayFactory::empty<float>();

    nd4j::ops::skipgram op;
    auto result = *op.execute({&target, &ngStarter, &indices, &codes, &syn0, &syn1, &syn1Neg, &expTable, &negTable, &alpha, &randomValue, &inferenceVector}, {}, {}, {false}, true);
    ASSERT_EQ(Status::OK(), result.status());

    auto row0 = *syn0.subarray({NDIndex::point(0), NDIndex::all()});
    auto row1 = *syn1.subarray({NDIndex::point(1), NDIndex::all()});

    row0.printIndexedBuffer("row0");
    row1.printIndexedBuffer("row1");

    ASSERT_EQ(exp0, row0);
    ASSERT_EQ(exp1, row1);
}

TEST_F(NlpTests, basic_sg_hs_test_2) {
    auto exp0 = NDArrayFactory::create<float>('c', {1, 10});
    auto exp1 = NDArrayFactory::create<float>('c', {1, 10});
    auto exp2 = NDArrayFactory::create<float>('c', {1, 10});

    exp0.assign(0.01f);
    exp1.assign(0.020005f);
    exp2.assign(0.019995f);

    auto target = NDArrayFactory::create<int>(0);
    auto ngStarter = NDArrayFactory::empty<int>();
    auto indices = NDArrayFactory::create<int>('c', {2}, {1, 2});
    auto codes = NDArrayFactory::create<int8_t>('c', {2}, {0, 1});
    auto syn0 = NDArrayFactory::create<float>('c', {100, 10});
    auto syn1 = NDArrayFactory::create<float>('c', {100, 10});
    auto syn1Neg = NDArrayFactory::empty<float>();
    auto expTable = NDArrayFactory::create<float>('c', {10000});
    auto negTable = NDArrayFactory::empty<float>();

    syn0.assign(0.01);
    syn1.assign(0.02);
    expTable.assign(0.5);

    auto alpha = NDArrayFactory::create<double>(0.001);
    auto randomValue = NDArrayFactory::create<Nd4jLong>(1L);
    auto inferenceVector = NDArrayFactory::empty<float>();

    nd4j::ops::skipgram op;
    auto result = *op.execute({&target, &ngStarter, &indices, &codes, &syn0, &syn1, &syn1Neg, &expTable, &negTable, &alpha, &randomValue, &inferenceVector}, {}, {}, {false}, true);
    ASSERT_EQ(Status::OK(), result.status());

    auto row0 = *syn0.subarray({NDIndex::point(0), NDIndex::all()});
    auto row1 = *syn1.subarray({NDIndex::point(1), NDIndex::all()});
    auto row2 = *syn1.subarray({NDIndex::point(2), NDIndex::all()});

    row0.printIndexedBuffer("row0");
    row1.printIndexedBuffer("row1");
    row2.printIndexedBuffer("row2");

    ASSERT_EQ(exp0, row0);
    ASSERT_EQ(exp1, row1);
    ASSERT_EQ(exp2, row2);
}

TEST_F(NlpTests, basic_sg_hs_ns_test_1) {
    auto target = NDArrayFactory::create<int>(0);
    auto ngStarter = NDArrayFactory::create<int>(1);
    auto indices = NDArrayFactory::create<int>('c', {5}, {1, 2, 3, 4, 5});
    auto codes = NDArrayFactory::create<int8_t>('c', {5}, {1, 1, 0, 1, 1});
    auto syn0 = NDArrayFactory::create<float>('c', {100, 150});
    auto syn1 = NDArrayFactory::create<float>('c', {100, 150});
    auto syn1Neg = NDArrayFactory::create<float>('c', {100, 150});
    auto expTable = NDArrayFactory::create<float>('c', {1000});
    auto negTable = NDArrayFactory::create<float>('c', {1000});
    negTable.linspace(1.0);

    auto alpha = NDArrayFactory::create<double>(1.25);
    auto randomValue = NDArrayFactory::create<Nd4jLong>(119L);
    auto inferenceVector = NDArrayFactory::empty<float>();

    nd4j::ops::skipgram op;
    auto result = *op.execute({&target, &ngStarter, &indices, &codes, &syn0, &syn1, &syn1Neg, &expTable, &negTable, &alpha, &randomValue, &inferenceVector}, {}, {3}, {false}, true);
    ASSERT_EQ(Status::OK(), result.status());
}

TEST_F(NlpTests, basic_sg_ns_test_1) {
    auto exp0 = NDArrayFactory::create<float>('c', {1, 10});

    exp0.assign(0.01);

    auto target = NDArrayFactory::create<int>(1);
    auto ngStarter = NDArrayFactory::create<int>(3);
    auto indices = NDArrayFactory::empty<int>();
    auto codes = NDArrayFactory::empty<int8_t>();
    auto syn0 = NDArrayFactory::create<float>('c', {10, 10});
    auto syn1 = NDArrayFactory::empty<float>();
    auto syn1Neg = NDArrayFactory::create<float>('c', {10, 10});
    auto expTable = NDArrayFactory::create<float>('c', {1000});
    auto negTable = NDArrayFactory::create<float>('c', {1000});

    auto syn1Neg2 = NDArrayFactory::create<float>('c', {10, 10});

    syn0.assign(0.01);
    syn1.assign(0.02);
    syn1Neg.assign(0.03);
    syn1Neg2.assign(0.03);
    expTable.assign(0.5);

    auto alpha = NDArrayFactory::create<double>(0.001);
    auto randomValue = NDArrayFactory::create<Nd4jLong>(2L);
    auto inferenceVector = NDArrayFactory::empty<float>();

    nd4j::ops::skipgram op;
    auto result = *op.execute({&target, &ngStarter, &indices, &codes, &syn0, &syn1, &syn1Neg, &expTable, &negTable, &alpha, &randomValue, &inferenceVector}, {}, {1}, {false}, true);
    ASSERT_EQ(Status::OK(), result.status());

    auto row0 = *syn0.subarray({NDIndex::point(1), NDIndex::all()});
    row0.printIndexedBuffer("row0");

    ASSERT_EQ(exp0, row0);
    ASSERT_FALSE(syn1Neg2.equalsTo(syn1Neg, 1e-6));
}

TEST_F(NlpTests, basic_cb_hs_test_1) {
    auto exp0 = NDArrayFactory::create<float>('c', {1, 10});
    auto exp1 = NDArrayFactory::create<float>('c', {1, 10});
    auto exp2 = NDArrayFactory::create<float>('c', {1, 10});

    exp0.assign(0.0095f);
    exp1.assign(0.019875f);
    exp2.assign(0.02f);

    auto target = NDArrayFactory::create<int>(0);
    auto ngStarter = NDArrayFactory::empty<int>();
    auto context = NDArrayFactory::create<int>('c', {3}, {0, 1, 2});
    auto indices = NDArrayFactory::create<int>('c', {2}, {4, 5});
    auto codes = NDArrayFactory::create<int8_t>('c', {2}, {1, 1});
    auto syn0 = NDArrayFactory::create<float>('c', {100, 10});
    auto syn1 = NDArrayFactory::create<float>('c', {100, 10});
    auto syn1Neg = NDArrayFactory::empty<float>();
    auto expTable = NDArrayFactory::create<float>('c', {10000});
    auto negTable = NDArrayFactory::empty<float>();

    syn0.assign(0.01);
    syn1.assign(0.02);
    expTable.assign(0.5);

    auto alpha = NDArrayFactory::create<double>(0.025);
    auto randomValue = NDArrayFactory::create<Nd4jLong>(2L);
    auto inferenceVector = NDArrayFactory::empty<float>();

    nd4j::ops::cbow op;
    auto result = *op.execute({&target, &ngStarter, &context, &indices, &codes, &syn0, &syn1, &syn1Neg, &expTable, &negTable, &alpha, &randomValue, &inferenceVector}, {}, {}, {true}, true);
    ASSERT_EQ(Status::OK(), result.status());

    auto row_s0_0 = *syn0.subarray({NDIndex::point(0), NDIndex::all()});
    auto row_s0_1 = *syn0.subarray({NDIndex::point(1), NDIndex::all()});
    auto row_s0_2 = *syn0.subarray({NDIndex::point(2), NDIndex::all()});

    auto row_s1_4 = *syn1.subarray({NDIndex::point(4), NDIndex::all()});
    auto row_s1_5 = *syn1.subarray({NDIndex::point(5), NDIndex::all()});
    auto row_s1_6 = *syn1.subarray({NDIndex::point(6), NDIndex::all()});

    row_s0_0.printIndexedBuffer("s0_0");
    row_s0_1.printIndexedBuffer("s0_1");
    row_s0_2.printIndexedBuffer("s0_2");

    row_s1_4.printIndexedBuffer("s1_4");
    row_s1_5.printIndexedBuffer("s1_5");
    row_s1_6.printIndexedBuffer("s1_6");

    ASSERT_EQ(exp0, row_s0_0);
    ASSERT_EQ(exp0, row_s0_1);
    ASSERT_EQ(exp0, row_s0_2);

    ASSERT_EQ(exp1, row_s1_4);
    ASSERT_EQ(exp1, row_s1_5);

    ASSERT_EQ(exp2, row_s1_6);
}

TEST_F(NlpTests, basic_cb_ns_test_1) {
    auto exp0 = NDArrayFactory::create<float>('c', {1, 10});
    auto exp1 = NDArrayFactory::create<float>('c', {1, 10});
    auto exp2 = NDArrayFactory::create<float>('c', {1, 10});

    exp0.assign(0.0096265625);
    exp1.assign(0.01);
    exp2.assign(0.030125f);

    auto target = NDArrayFactory::create<int>(0);
    auto ngStarter = NDArrayFactory::create<int>(6);
    auto context = NDArrayFactory::create<int>('c', {3}, {0, 1, 2});
    auto indices = NDArrayFactory::empty<int>();
    auto codes = NDArrayFactory::empty<int8_t>();
    auto syn0 = NDArrayFactory::create<float>('c', {100, 10});
    auto syn1 = NDArrayFactory::create<float>('c', {100, 10});
    auto syn1Neg = NDArrayFactory::create<float>('c', {100, 10});
    auto expTable = NDArrayFactory::create<float>('c', {10000});
    auto negTable = NDArrayFactory::create<float>('c', {100000});

    syn0.assign(0.01);
    syn1.assign(0.02);
    syn1Neg.assign(0.03);
    expTable.assign(0.5);

    auto alpha = NDArrayFactory::create<double>(0.025);
    auto randomValue = NDArrayFactory::create<Nd4jLong>(2L);
    auto inferenceVector = NDArrayFactory::empty<float>();

    nd4j::ops::cbow op;
    auto result = *op.execute({&target, &ngStarter, &context, &indices, &codes, &syn0, &syn1, &syn1Neg, &expTable, &negTable, &alpha, &randomValue, &inferenceVector}, {}, {0, 2}, {true}, true);
    ASSERT_EQ(Status::OK(), result.status());

    auto row_s0_0 = *syn0.subarray({NDIndex::point(0), NDIndex::all()});
    auto row_s0_1 = *syn0.subarray({NDIndex::point(1), NDIndex::all()});
    auto row_s0_2 = *syn0.subarray({NDIndex::point(2), NDIndex::all()});

    auto row_s1_4 = *syn1.subarray({NDIndex::point(4), NDIndex::all()});
    auto row_s1_5 = *syn1.subarray({NDIndex::point(5), NDIndex::all()});
    auto row_s1_6 = *syn1Neg.subarray({NDIndex::point(6), NDIndex::all()});

    row_s0_0.printIndexedBuffer("s0_0");
    row_s0_1.printIndexedBuffer("s0_1");
    row_s0_2.printIndexedBuffer("s0_2");

    row_s1_4.printIndexedBuffer("s1_4");
    row_s1_5.printIndexedBuffer("s1_5");
    row_s1_6.printIndexedBuffer("s1_6");

    ASSERT_EQ(exp0, row_s0_0);
    ASSERT_EQ(exp0, row_s0_1);
    ASSERT_EQ(exp0, row_s0_2);

    ASSERT_EQ(exp2, row_s1_6);
}