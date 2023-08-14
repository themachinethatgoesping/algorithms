# SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
#
# SPDX-License-Identifier: MPL-2.0

""" tests for the tutorial class
"""

# import time
from pytest import approx

# import numpy as np



class TestAlgorithmsDummy:
    """class for grouping (test sections)"""

    def test_all_algorithm_modules_should_be_importable(self):

        import themachinethatgoesping.algorithms as alg
        
        import themachinethatgoesping.algorithms.geoprocessing as gproc
        import themachinethatgoesping.algorithms.geoprocessing.datastructures as structures
        import themachinethatgoesping.algorithms.geoprocessing.raytracers as raytracers
        
        
        
