package com.ecoomerce.sportscenter.controller;

import com.ecoomerce.sportscenter.model.BrandResponse;
import com.ecoomerce.sportscenter.model.ProductResponse;
import com.ecoomerce.sportscenter.model.TypeResponse;
import com.ecoomerce.sportscenter.service.BrandService;
import com.ecoomerce.sportscenter.service.ProductService;
import com.ecoomerce.sportscenter.service.TypeService;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.PageImpl;
import org.springframework.data.domain.PageRequest;
import org.springframework.data.web.PageableHandlerMethodArgumentResolver;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.setup.MockMvcBuilders;

import java.util.List;

import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.when;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.get;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.jsonPath;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;

@ExtendWith(MockitoExtension.class)
class ProductControllerTest {

    private MockMvc mockMvc;

    @Mock
    private ProductService productService;

    @Mock
    private TypeService typeService;

    @Mock
    private BrandService brandService;

    @InjectMocks
    private ProductController productController;

    @BeforeEach
    void setUp() {
        mockMvc = MockMvcBuilders.standaloneSetup(productController)
                .setCustomArgumentResolvers(new PageableHandlerMethodArgumentResolver())
                .build();
    }

    @Test
    void getProductById_returnsOk() throws Exception {
        when(productService.getProductById(1)).thenReturn(
                ProductResponse.builder().id(1).name("Ball").build());

        mockMvc.perform(get("/api/products/1"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.id").value(1))
                .andExpect(jsonPath("$.name").value("Ball"));
    }

    @Test
    void getProducts_defaultPaging() throws Exception {
        Page<ProductResponse> page = new PageImpl<>(
                List.of(ProductResponse.builder().id(1).name("Ball").build()),
                PageRequest.of(0, 10),
                1);
        when(productService.getProducts(any())).thenReturn(page);

        mockMvc.perform(get("/api/products"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.content[0].name").value("Ball"));
    }

    @Test
    void getProducts_searchByKeyword() throws Exception {
        when(productService.searchProductsByName("ball"))
                .thenReturn(List.of(ProductResponse.builder().id(1).name("Ball").build()));

        mockMvc.perform(get("/api/products").param("keyword", "ball"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.content[0].name").value("Ball"));
    }

    @Test
    void getProducts_searchByBrand() throws Exception {
        when(productService.searchProductsByBrand(2))
                .thenReturn(List.of(ProductResponse.builder().id(1).build()));

        mockMvc.perform(get("/api/products").param("brandId", "2"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.content[0].id").value(1));
    }

    @Test
    void getProducts_searchByType() throws Exception {
        when(productService.searchProductsByType(3))
                .thenReturn(List.of(ProductResponse.builder().id(1).build()));

        mockMvc.perform(get("/api/products").param("typeId", "3"))
                .andExpect(status().isOk());
    }

    @Test
    void getProducts_searchByBrandAndType() throws Exception {
        when(productService.searchProductsByBrandandType(2, 3))
                .thenReturn(List.of(ProductResponse.builder().id(1).build()));

        mockMvc.perform(get("/api/products").param("brandId", "2").param("typeId", "3"))
                .andExpect(status().isOk());
    }

    @Test
    void getProducts_searchByBrandTypeAndKeyword() throws Exception {
        when(productService.searchProductsByBrandTypeAndName(eq(2), eq(3), eq("ball")))
                .thenReturn(List.of(ProductResponse.builder().id(1).build()));

        mockMvc.perform(get("/api/products")
                        .param("brandId", "2")
                        .param("typeId", "3")
                        .param("keyword", "ball"))
                .andExpect(status().isOk());
    }

    @Test
    void getProducts_sortDesc() throws Exception {
        Page<ProductResponse> page = new PageImpl<>(List.of(), PageRequest.of(0, 10), 0);
        when(productService.getProducts(any())).thenReturn(page);

        mockMvc.perform(get("/api/products").param("order", "desc").param("sort", "price"))
                .andExpect(status().isOk());
    }

    @Test
    void getBrands_returnsList() throws Exception {
        when(brandService.getAllBrands()).thenReturn(List.of(
                BrandResponse.builder().id(1).name("Nike").build()));

        mockMvc.perform(get("/api/products/brands"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$[0].name").value("Nike"));
    }

    @Test
    void getTypes_returnsList() throws Exception {
        when(typeService.getAllTypes()).thenReturn(List.of(
                TypeResponse.builder().id(1).name("Shoes").build()));

        mockMvc.perform(get("/api/products/types"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$[0].name").value("Shoes"));
    }

    @Test
    void searchProducts_returnsList() throws Exception {
        when(productService.searchProductsByName("ball"))
                .thenReturn(List.of(ProductResponse.builder().id(1).name("Ball").build()));

        mockMvc.perform(get("/api/products/search").param("keyword", "ball"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$[0].name").value("Ball"));
    }
}
